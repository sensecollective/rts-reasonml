/* externals */
module Date = {
  [@bs.val] external now : unit => float = "Date.now";
};

/* main app state */
type windowT = {
  mutable width: int,
  mutable height: int
};

type stateT = {window: windowT};

let state: stateT = {
  window: {
    width: 0,
    height: 0
  }
};

exception NoGL;

exception No2D;

exception NoProgram;

let counter = ref(0);

let countInvocations =
  Memoize.memoize(
    1,
    a => {
      counter := counter^ + 1;
      ("count", a, counter^);
    }
  );

countInvocations(0);

let getShaderProgram = gl =>
  Memoize.memoize(
    2,
    (fg, bg) => {
      let (uniforms, programSource) = ShaderExample.makeProgramSource(fg, bg);
      let vertexShaderSource = programSource.vertexShader;
      let fragmentShaderSource = programSource.fragmentShader;
      Js.log("Vertex shader:");
      Js.log(MyString.lineNumbers(vertexShaderSource));
      let vertexShader =
        WebGL2.createShader(
          gl,
          WebGL2.getVERTEX_SHADER(gl),
          vertexShaderSource
        );
      Js.log("Fragment shader:");
      Js.log(MyString.lineNumbers(fragmentShaderSource));
      let fragmentShader =
        WebGL2.createShader(
          gl,
          WebGL2.getFRAGMENT_SHADER(gl),
          fragmentShaderSource
        );
      let program =
        switch (vertexShader, fragmentShader) {
        | (Some(vs), Some(fs)) => WebGL2.createProgram(gl, vs, fs)
        | _ => None
        };
      (uniforms, program);
    }
  );

let setupDocument = () => {
  Document.setMargin(Document.getStyle(Document.body), "0px");
  Document.setOverflow(Document.getStyle(Document.body), "hidden");
  let canvas = Document.createElement("canvas");
  let showError = msg => {
    let body = Document.body;
    Document.setInnerHTML(body, "<h1 style='color: red;'>" ++ msg ++ "</h1>");
  };
  let _ = Document.appendChild(canvas);
  let gl =
    switch (Js.Nullable.to_opt(WebGL2.getContext(canvas, "webgl2"))) {
    | Some(gl) => gl
    | None =>
      showError("No WebGL2!");
      raise(NoGL);
    };
  let setCanvasSize = (_) => {
    let width = Document.getWidth(Document.window);
    let height = Document.getHeight(Document.window);
    Document.setWidth(canvas, width);
    Document.setHeight(canvas, height);
    state.window.width = width;
    state.window.height = height;
  };
  Document.addEventListener(Document.window, "resize", setCanvasSize);
  setCanvasSize();
  Document.addEventListener(
    Document.window,
    "DOMContentLoaded",
    setCanvasSize
  );
  ConfigUI.init();
  (canvas, gl);
};

let run = (gl, time) => {
  let geometryType = ConfigVars.geometryType#get();
  let fg = ConfigVars.foregroundColor#get();
  let bg = ConfigVars.backgroundColor#get();
  let geof =
    switch geometryType {
    | "Box" => Three.createBoxGeometry
    | "Sphere" => Three.createSphereGeometry
    | "Plane" => Three.createPlaneGeometry
    | _ => Three.createBoxGeometry
    };
  switch (getShaderProgram(gl, fg, bg)) {
  | (uniforms, Some(p)) =>
    WebGL2.testProgram(
      gl,
      p,
      uniforms,
      state.window.width,
      state.window.height,
      time,
      fg,
      bg,
      geof,
      Three.getObjectMatrix,
      Three.getViewMatrices
    )
  | (_, None) => raise(NoProgram)
  };
};

let rec renderLoop = (startTime, canvas, gl, startIteration) => {
  let t = Date.now() -. startTime;
  run(gl, t /. 1000.0);
  let currentIteration = Document.iteration(Document.window);
  if (currentIteration == startIteration) {
    Document.requestAnimationFrame(() => renderLoop(startTime, canvas, gl, startIteration));
  } else {
    let _ = Document.removeChild(canvas);
    Js.log(
      "exiting render loop "
      ++ string_of_int(startIteration)
      ++ " due to hot reload"
    );
  };
};

let main = (_) => {
  let (canvas, gl) = setupDocument();
  let startTime = Date.now();
  let startIteration = Document.iteration(Document.window);
  renderLoop(startTime, canvas, gl, startIteration);
  () => {
    Js.log("destroying last app generation");
    ConfigUI.destroy();
  };
};
