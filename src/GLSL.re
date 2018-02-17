module SS = Set.Make(String);

let version = "#version 300 es";

let vertexPrelude = {|
#define VARYING out

|};

let fragmentPrelude = {|
#define VARYING in|};

let precisionQuantifier = "mediump";

let precision = "precision " ++ precisionQuantifier ++ " float;";

let newline = "\n";

let indent = "  ";

exception GLSLTypeError(string);

type glslTypeT =
  | Void
  | Int
  | Float
  | Vec2
  | Vec3
  | Vec4
  | Mat2
  | Mat3
  | Mat4
  | Sampler2D;

type varT =
  | Builtin
  | Attribute
  | Uniform
  | Varying
  | Variable
  | Output
  | Argument;

type varExprT = (varT, glslTypeT, string);

include GLSLSwizzleFormat;

type glslVariantTypeT =
  [
    | `Untyped
    | `Void
    | `Int
    | `Float
    | `Vec2
    | `Vec3
    | `Vec4
    | `Mat2
    | `Mat3
    | `Mat4
    | `Sampler2D
  ] as 'a;

type glslVariantTypeUBT('a) = [< glslVariantTypeT] as 'a;

type glslVariantTypeLBT('a) = [> glslVariantTypeT] as 'a;

type nodeType('a, 'b) = 'a => 'b
constraint 'a = glslVariantTypeUBT('a)
constraint 'b = glslVariantTypeLBT('b);

type leftExprT =
  | Var(varExprT)
  | Swizzle(varExprT, swizzleT)
and lT = leftExprT
/* list(rT) in funExprT is too wide,
 * but it makes it easy to use vars as arguments and
 * as right expressions interchangeably */
and funExprT = (glslTypeT, string, list(rT), gRootT)
/* rT: Translated from https://github.com/kovasb/gamma/blob/master/src/gamma/ast.cljs */
and rT =
  /* | TypeError */
  | RVar(varExprT)
  | RSwizzle(rT, swizzleT)
  | ImmediateFloat(float)
  | ImmediateInt(int)
  | Inc(rT)
  | Dec(rT)
  | PreInc(rT)
  | PreDec(rT)
  | Not(rT)
  /* | PlusMinus(rT('a,'b)) */
  | Mul(rT, rT)
  | Div(rT, rT)
  | Plus(rT, rT)
  | Minus(rT, rT)
  | LessThan(rT, rT)
  | GreaterThan(rT, rT)
  | LessThanOrEqual(rT, rT)
  | GreaterThanOrEqual(rT, rT)
  | Equal(rT, rT)
  | NotEqual(rT, rT)
  | And(rT, rT)
  | Or(rT, rT)
  | Xor(rT, rT)
  | Ternary(rT, rT, rT)
  | BuiltinFun(string, list(rT))
  | BuiltinFun1(string, rT)
  | BuiltinFun2(string, rT, rT)
  | BuiltinFun3(string, rT, rT, rT)
  | BuiltinFun4(string, rT, rT, rT, rT)
  /*| Texture(rT([ | `Sampler2D | `Vec2]), rT([ | `Sampler2D | `Vec2]))*/
  | CustomFun(funExprT, list(rT))
and rightExprT = rT
and trT('a) =
  | Typed('a, rT)
  | Untyped(rT)
and statementT =
  | Assignment(lT, rT)
  | DeclAssignment(glslTypeT, lT, rT)
  | IfStatement(rT, gRootT)
  | IfElseStatement(rT, gRootT, gRootT)
  | Return(rT)
  | Discard
and gRootT = list(statementT);

/*
 type cfun1 = 'a => trT('b);

 type cfun2('a, 'b, 'c) =  => trT('c);
 */
let unused = x => x;

let genericexprlist = (l, e) =>
  switch (List.hd(l)) {
  | Typed(`Float, _) => Typed(`Float, unused(e))
  | Typed(`Vec2, _) => Typed(`Vec2, unused(e))
  | Typed(`Vec3, _) => Typed(`Vec3, unused(e))
  | Typed(`Vec4, _) => Typed(`Vec4, unused(e))
  | Untyped(_) => raise(GLSLTypeError("type mismatch"))
  };

let genericexpr1 = (l, e) =>
  switch l {
  | Typed(`Float, _) => Typed(`Float, unused(e))
  | Typed(`Vec2, _) => Typed(`Vec2, unused(e))
  | Typed(`Vec3, _) => Typed(`Vec3, unused(e))
  | Typed(`Vec4, _) => Typed(`Vec4, unused(e))
  | Untyped(_) => raise(GLSLTypeError("type mismatch"))
  };

let genericexpr1float = (l, e) =>
  switch l {
  | Typed(`Float, _) => Typed(`Float, unused(e))
  | Typed(`Vec2, _) => Typed(`Float, unused(e))
  | Typed(`Vec3, _) => Typed(`Float, unused(e))
  | Typed(`Vec4, _) => Typed(`Float, unused(e))
  | Untyped(_) => raise(GLSLTypeError("type mismatch"))
  };

let genericmath2 = (l, r, e) =>
  switch (l, r) {
  | (Typed(`Float, _), Typed(`Float, _)) => Typed(`Float, unused(e))
  | (Typed(`Vec2, _), Typed(`Vec2, _)) => Typed(`Vec2, unused(e))
  | (Typed(`Vec3, _), Typed(`Vec3, _)) => Typed(`Vec3, unused(e))
  | (Typed(`Vec4, _), Typed(`Vec4, _)) => Typed(`Vec4, unused(e))
  | (Typed(`Mat2, _), Typed(`Mat2, _)) => Typed(`Mat2, unused(e))
  | (Typed(`Mat3, _), Typed(`Mat3, _)) => Typed(`Mat3, unused(e))
  | (Typed(`Mat4, _), Typed(`Mat4, _)) => Typed(`Mat4, unused(e))
  | _ => raise(GLSLTypeError("type mismatch"))
  };

let genericexpr2 = (l, r, e) =>
  switch (l, r) {
  | (Typed(`Float, _), Typed(`Float, _)) => Typed(`Float, unused(e))
  | (Typed(`Vec2, _), Typed(`Vec2, _)) => Typed(`Vec2, unused(e))
  | (Typed(`Vec3, _), Typed(`Vec3, _)) => Typed(`Vec3, unused(e))
  | (Typed(`Vec4, _), Typed(`Vec4, _)) => Typed(`Vec4, unused(e))
  | _ => raise(GLSLTypeError("type mismatch"))
  };

let genericexpr2float = (l, r, e) =>
  switch (l, r) {
  | (Typed(`Float, _), Typed(`Float, _)) => Typed(`Float, unused(e))
  | (Typed(`Vec2, _), Typed(`Vec2, _)) => Typed(`Float, unused(e))
  | (Typed(`Vec3, _), Typed(`Vec3, _)) => Typed(`Float, unused(e))
  | (Typed(`Vec4, _), Typed(`Vec4, _)) => Typed(`Float, unused(e))
  | _ => raise(GLSLTypeError("type mismatch"))
  };

/*
 type nodeT =
   | StatementNode(statementT)
   | LExprNode(leftExprT)
   | RExprNode(rightExprT)
   | VarNode(varExprT)
   | SwizzleNode(swizzleT);
   */
let glslTypeString = t =>
  switch t {
  | Void => "void"
  | Int => "int"
  | Float => "float"
  | Vec2 => "vec2"
  | Vec3 => "vec3"
  | Vec4 => "vec4"
  | Mat2 => "mat2"
  | Mat3 => "mat3"
  | Mat4 => "mat4"
  | Sampler2D => "sampler2D"
  };

type uniformInputT = {
  time: float,
  tick: float,
  width: int,
  height: int,
  modelViewMatrix: array(float),
  projectionMatrix: array(float)
};

type uniformBlockT = list((rT, uniformInputT => array(float)));

type shaderMainT = gRootT;

/* TODO: should probably be an object */
type transformer('a) = {
  var: (transformer('a), varExprT) => 'a,
  combine: (transformer('a), list('a)) => 'a,
  rexprCombine: (transformer('a), list('a)) => 'a,
  lExpr: (transformer('a), lT) => 'a,
  rExpr: (transformer('a), rT) => 'a,
  tree: (transformer('a), list(statementT)) => 'a,
  tfun: (transformer('a), shaderMainT) => 'a
};

let combine = (_, l) => List.fold_left((++), "", l);

let fmtTransformer = {
  var: (_, expr) => {
    let (_, _, name) = expr;
    name;
  },
  combine,
  rexprCombine: (t, l) => t.combine(t, ["(", t.combine(t, l), ")"]),
  lExpr: (t, expr) =>
    t.combine(
      t,
      switch expr {
      | Var(x) => [t.var(t, x)]
      | Swizzle((_, _, name), swizzle) => [name, ".", fmtSwizzle(swizzle)]
      }
    ),
  rExpr: (t, expr) =>
    t.rexprCombine(
      t,
      switch expr {
      | RVar(x) => [t.var(t, x)]
      | RSwizzle(expr, swizzle) => [t.rExpr(t, expr), ".", fmtSwizzle(swizzle)]
      | ImmediateFloat(e) => [string_of_float(e)]
      | ImmediateInt(e) => [string_of_int(e)]
      | Inc(l) => ["(", t.rExpr(t, l), "++)"]
      | Dec(l) => ["(", t.rExpr(t, l), "--)"]
      | PreInc(l) => ["(++", t.rExpr(t, l), ")"]
      | PreDec(l) => ["(--", t.rExpr(t, l), ")"]
      | Not(l) => ["(!", t.rExpr(t, l), ")"]
      | Mul(l, r) => ["(", t.rExpr(t, l), " * ", t.rExpr(t, r), ")"]
      | Div(l, r) => ["(", t.rExpr(t, l), " / ", t.rExpr(t, r), ")"]
      | Plus(l, r) => ["(", t.rExpr(t, l), " + ", t.rExpr(t, r), ")"]
      | Minus(l, r) => ["(", t.rExpr(t, l), " - ", t.rExpr(t, r), ")"]
      | LessThan(l, r) => ["(", t.rExpr(t, l), " < ", t.rExpr(t, r), ")"]
      | GreaterThan(l, r) => ["(", t.rExpr(t, l), " > ", t.rExpr(t, r), ")"]
      | LessThanOrEqual(l, r) => [
          "(",
          t.rExpr(t, l),
          " <= ",
          t.rExpr(t, r),
          ")"
        ]
      | GreaterThanOrEqual(l, r) => [
          "(",
          t.rExpr(t, l),
          " >= ",
          t.rExpr(t, r),
          ")"
        ]
      | Equal(l, r) => ["(", t.rExpr(t, l), " == ", t.rExpr(t, r), ")"]
      | NotEqual(l, r) => ["(", t.rExpr(t, l), " != ", t.rExpr(t, r), ")"]
      | And(l, r) => ["(", t.rExpr(t, l), " && ", t.rExpr(t, r), ")"]
      | Or(l, r) => ["(", t.rExpr(t, l), " || ", t.rExpr(t, r), ")"]
      | Xor(l, r) => ["(", t.rExpr(t, l), " ^^ ", t.rExpr(t, r), ")"]
      | Ternary(l, r1, r2) => [
          "(",
          t.rExpr(t, l),
          " ? ",
          t.rExpr(t, r1),
          " : ",
          t.rExpr(t, r2),
          ")"
        ]
      | CustomFun((_, name, _, _), l) =>
        let args = List.map(x => t.rExpr(t, x), l);
        let args =
          List.fold_left(
            (x, y) => List.concat([x, [",", y]]),
            [List.hd(args)],
            List.tl(args)
          );
        List.concat([[name, "("], args, [")"]]);
      | BuiltinFun(name, l) =>
        let args = List.map(x => t.rExpr(t, x), l);
        let args =
          List.fold_left(
            (x, y) => List.concat([x, [",", y]]),
            [List.hd(args)],
            List.tl(args)
          );
        List.concat([[name, "("], args, [")"]]);
      | BuiltinFun1(name, l) => [name, "(", t.rExpr(t, l), ")"]
      | BuiltinFun2(name, l, r) => [
          name,
          "(",
          t.rExpr(t, l),
          ", ",
          t.rExpr(t, r),
          ")"
        ]
      /*| Texture(l, r) => [
          "texture",
          "(",
          t.rExpr(t, l),
          ", ",
          t.rExpr(t, r),
          ")"
        ]*/
      | BuiltinFun3(name, l, r, r2) => [
          name,
          "(",
          t.rExpr(t, l),
          ", ",
          t.rExpr(t, r),
          ", ",
          t.rExpr(t, r2),
          ")"
        ]
      | BuiltinFun4(name, l, r, r2, r3) => [
          name,
          "(",
          t.rExpr(t, l),
          ", ",
          t.rExpr(t, r),
          ", ",
          t.rExpr(t, r2),
          ", ",
          t.rExpr(t, r3),
          ")"
        ]
      }
    ),
  tree: (t, tree) =>
    t.combine(
      t,
      List.map(
        stmt =>
          t.combine(
            t,
            [
              indent,
              switch stmt {
              | Assignment(left, right) =>
                t.combine(t, [t.lExpr(t, left), " = ", t.rExpr(t, right)])
              | DeclAssignment(vart, left, right) =>
                t.combine(
                  t,
                  [
                    glslTypeString(vart),
                    " ",
                    t.lExpr(t, left),
                    " = ",
                    t.rExpr(t, right)
                  ]
                )
              | Return(right) =>
                t.combine(t, ["return ", t.rExpr(t, right), ";"])
              | IfStatement(l, body) =>
                let body = t.tree(t, body);
                t.combine(
                  t,
                  ["if (", t.rExpr(t, l), ") {", newline, body, newline, "}"]
                );
              | IfElseStatement(l, b1, b2) =>
                let b1 = t.tree(t, b1);
                let b2 = t.tree(t, b2);
                t.combine(
                  t,
                  [
                    "if (",
                    t.rExpr(t, l),
                    ") {",
                    newline,
                    b1,
                    newline,
                    "} else {",
                    newline,
                    b2,
                    newline,
                    "}"
                  ]
                );
              | Discard => "discard"
              },
              ";",
              newline
            ]
          ),
        tree
      )
    ),
  tfun: (t, tree) =>
    t.combine(t, ["void main() {", newline, t.tree(t, tree), "}"])
};

let fmtFun = gf => fmtTransformer.tfun(fmtTransformer, gf);

let orderVars = (fmt, vars) =>
  List.map(fmt, List.sort_uniq(Pervasives.compare, vars));

let getVarOfType = (vart, gf) => {
  let ar = ref([]);
  let walkTransformer = {
    ...fmtTransformer,
    var: (t, expr) => {
      switch expr {
      | (x, t, name) when x == vart => ar := [(t, name), ...ar^]
      | _ => ()
      };
      fmtTransformer.var(t, expr);
    }
  };
  let _ = walkTransformer.tfun(walkTransformer, gf);
  ar^;
};

let getFunctions = gf => {
  let ar = ref([]);
  let walkTransformer = {
    ...fmtTransformer,
    rExpr: (t, expr) => {
      switch expr {
      | CustomFun((a, b, c, body), _) =>
        ar := [(a, b, c, body), ...ar^];
        let _ = t.tree(t, body);
        ();
      | _ => ()
      };
      fmtTransformer.rExpr(t, expr);
    }
  };
  let _ = walkTransformer.tfun(walkTransformer, gf);
  ar^;
};

let getAttributes = gf => getVarOfType(Attribute, gf);

let getVaryings = gf => getVarOfType(Varying, gf);

let getUniforms = gf => getVarOfType(Uniform, gf);

let getOutputs = gf => getVarOfType(Output, gf);

/*
 type fft = list((glslTypeT, SS.elt, list(trT('a,'b)), gRootT)) => string;

 type fftinner =
   ((glslTypeT, SS.elt, list(rT), list(statementT))) => SS.elt;
   */
let formatFunctions = attrs => {
  let t = fmtTransformer;
  let fns = ref(SS.empty);
  let formatFunction = attr => {
    let (vart, name, argvars, body) = attr;
    let argvars =
      List.map(
        x =>
          switch x {
          | RVar((_, vart, name)) =>
            t.combine(t, [glslTypeString(vart), " ", name])
          | _ => ""
          },
        argvars
      );
    let body = t.tree(t, body);
    let fn =
      t.combine(
        t,
        List.concat([
          [glslTypeString(vart), " ", name, "("],
          argvars,
          [") {", newline, body, newline, "}"]
        ])
      );
    switch (SS.find(fn, fns^)) {
    | _ => ""
    | exception Not_found =>
      fns := SS.add(fn, fns^);
      fn;
    };
  };
  String.concat(newline, List.map(formatFunction, attrs));
};

let formatAttributes = attrs => {
  let formatAttribute = attr => {
    let (t, name) = attr;
    "in " ++ glslTypeString(t) ++ " " ++ name ++ ";";
  };
  String.concat(newline, orderVars(formatAttribute, attrs));
};

let formatVaryings = attrs => {
  let formatVarying = attr => {
    let (t, name) = attr;
    "VARYING " ++ glslTypeString(t) ++ " " ++ name ++ ";";
  };
  String.concat(newline, orderVars(formatVarying, attrs));
};

let formatUniforms = attrs => {
  let formatUniform = attr => {
    let (t, name) = attr;
    "  "
    ++ precisionQuantifier
    ++ " "
    ++ glslTypeString(t)
    ++ " "
    ++ name
    ++ ";";
  };
  "layout(std140) uniform u_PerScene {"
  ++ newline
  ++ String.concat(newline, List.map(formatUniform, attrs)) /*orderVars(formatUniform, attrs))*/
  ++ newline
  ++ "};";
};

let formatOutputs = attrs => {
  let formatAttribute = attr => {
    let (t, name) = attr;
    "out " ++ glslTypeString(t) ++ " " ++ name ++ ";";
  };
  String.concat(newline, orderVars(formatAttribute, attrs));
};

let getShader = (prelude, uniforms, varyings, main) =>
  version
  ++ newline
  ++ newline
  ++ precision
  ++ newline
  ++ prelude
  ++ formatAttributes(getAttributes(main))
  ++ newline
  ++ newline
  ++ formatVaryings(varyings)
  ++ newline
  ++ newline
  ++ formatUniforms(uniforms)
  ++ newline
  ++ newline
  ++ formatOutputs(getOutputs(main))
  ++ newline
  ++ newline
  ++ formatFunctions(getFunctions(main))
  ++ newline
  ++ newline
  ++ fmtFun(main);

type programT = {
  attributes: list((glslTypeT, string)),
  uniforms: list((glslTypeT, string)),
  varyings: list((glslTypeT, string)),
  outputs: list((glslTypeT, string)),
  vertexShader: string,
  fragmentShader: string
};

let rightToLeft = right => {
  switch right {
  | RVar(x) => (x, Var(x))
  | RSwizzle(RVar(x), s) => (x, Swizzle(x, s))
  | _ => raise(GLSLTypeError("invalid left hand side"))
  };
};

let getProgram = (uniformBlock, vsmain, fsmain) => {
  /*
   let uniforms = List.concat([getUniforms(vsmain), getUniforms(fsmain)]);
   */
  let uf = ((e, _)) => {
    let ((_, x, y), _) = rightToLeft(e);
    (x, y);
  };
  let uniforms = List.map(uf, uniformBlock);
  let varyings = List.concat([getVaryings(vsmain), getVaryings(fsmain)]);
  {
    attributes: getAttributes(vsmain),
    outputs: getOutputs(fsmain),
    uniforms,
    varyings,
    vertexShader: getShader(vertexPrelude, uniforms, varyings, vsmain),
    fragmentShader: getShader(fragmentPrelude, uniforms, varyings, fsmain)
  };
};

let assign = (hasVar, addVar, dest, src) => {
  let ((vart, glslt, name), left) = rightToLeft(dest);
  if (hasVar(name) || vart != Variable) {
    Assignment(left, src);
  } else {
    if (vart == Variable) {
      addVar(name);
    };
    DeclAssignment(glslt, left, src);
  };
};

let symCounter = ref(0);

let genSym = () => {
  symCounter := symCounter^ + 1;
  "v_" ++ string_of_int(symCounter^);
};

let wrap = x => {
  let gt =
    switch x {
    | RVar((_, vart, _)) =>
      switch vart {
      | Void => `Void
      | Int => `Int
      | Float => `Float
      | Vec2 => `Vec2
      | Vec3 => `Vec3
      | Vec4 => `Vec4
      | Mat2 => `Mat2
      | Mat3 => `Mat3
      | Mat4 => `Mat4
      | Sampler2D => `Sampler2D
      }
    | _ => raise(GLSLTypeError("type mismatch"))
    };
  Typed(gt, x);
};

let attr = (t, name) => wrap(RVar((Attribute, t, name)));

let varying = (t, name) => wrap(RVar((Varying, t, name)));

let builtin = (t, name) => wrap(RVar((Builtin, t, name)));

let uniform = (t, name) => wrap(RVar((Uniform, t, name)));

let var = (t, name) => wrap(RVar((Variable, t, name)));

let output = (t, name) => wrap(RVar((Output, t, name)));

let gfun = (t, name) => (t, name);

let arg = (t, name) => wrap(RVar((Argument, t, name)));

let floatfun = name => gfun(Float, name);

let vec2fun = name => gfun(Vec2, name);

let vec3fun = name => gfun(Vec3, name);

let vec4fun = name => gfun(Vec4, name);

let mat2fun = name => gfun(Mat2, name);

let mat3fun = name => gfun(Mat3, name);

let mat4fun = name => gfun(Mat4, name);

let floatarg = name => arg(Float, name);

let vec2arg = name => arg(Vec2, name);

let vec3arg = name => arg(Vec3, name);

let vec4arg = name => arg(Vec4, name);

let mat2arg = name => arg(Mat2, name);

let mat3arg = name => arg(Mat3, name);

let mat4arg = name => arg(Mat4, name);

let fundecl = ((funt, name), argtypes, body, args) =>
  CustomFun((funt, name, argtypes, body), args);

let floatattr = name => attr(Float, name);

let vec2attr = name => attr(Vec2, name);

let vec3attr = name => attr(Vec3, name);

let vec4attr = name => attr(Vec4, name);

let mat2attr = name => attr(Mat2, name);

let mat3attr = name => attr(Mat3, name);

let mat4attr = name => attr(Mat4, name);

let floatvarying = name => varying(Float, name);

let vec2varying = name => varying(Vec2, name);

let vec3varying = name => varying(Vec3, name);

let vec4varying = name => varying(Vec4, name);

let mat2varying = name => varying(Mat2, name);

let mat3varying = name => varying(Mat3, name);

let mat4varying = name => varying(Mat4, name);

let floatuniform = name => uniform(Float, name);

let vec2uniform = name => uniform(Vec2, name);

let vec3uniform = name => uniform(Vec3, name);

let vec4uniform = name => uniform(Vec4, name);

let mat2uniform = name => uniform(Mat2, name);

let mat3uniform = name => uniform(Mat3, name);

let mat4uniform = name => uniform(Mat4, name);

let sampler2Duniform = name => uniform(Sampler2D, name);

let floatvar = name => var(Float, name);

let vec2var = name => var(Vec2, name);

let vec3var = name => var(Vec3, name);

let vec4var = name => var(Vec4, name);

let gl_Position = builtin(Vec4, "gl_Position");

let gl_FragCoord = builtin(Vec4, "gl_FragCoord");

let gl_FragColor = builtin(Vec4, "gl_FragColor");

let outColor = output(Vec4, "outColor");

let body = x => x;

let arStack = ref([]);

let ar = ref([]);

let vars = ref(SS.empty);

let add = x => {
  ar := [x, ...ar^];
  ();
};

let finish = () => {
  let value = List.rev(ar^);
  ar := [];
  vars := SS.empty;
  value;
};

let push = () => {
  arStack := [ar^, ...arStack^];
  ar := [];
};

let pop = () => {
  let ret = finish();
  ar := List.hd(arStack^);
  arStack := List.tl(arStack^);
  ret;
};

let hasVar = var =>
  switch (SS.find(var, vars^)) {
  | _ => true
  | exception Not_found => false
  };

let addVar = var => {
  vars := SS.add(var, vars^);
  ();
};

let gm2 = genericmath2;

let g1 = genericexpr1;

let g2 = genericexpr2;

let glist = genericexprlist;

/*
 type assignType = (trT('a,'b), trT('b)) => unit
 constraint 'a = glslVariantTypeUBT
 constraint 'b = glslVariantTypeUBT('b);
 */
let u = te =>
  switch te {
  | Typed(_, e) => e
  | Untyped(e) => e
  };

let (=@) = (l, r) => add(assign(hasVar, addVar, u(l), u(r)));

let (++) = l => g1(l, Inc(u(l)));

let (--) = l => g1(l, Dec(u(l)));

let (+++) = l => g1(l, PreInc(u(l)));

let (---) = l => g1(l, PreDec(u(l)));

let (!) = l => g1(l, Not(u(l)));

let ( * ) = (l, r) => gm2(l, r, Mul(u(l), u(r)));

let ( *= ) = (l, r) => l =@ l * r;

let (/) = (l, r) => gm2(l, r, Div(u(l), u(r)));

let (/=) = (l, r) => l =@ l / r;

let (+) = (l, r) => gm2(l, r, Plus(u(l), u(r)));

let (+=) = (l, r) => l =@ l + r;

let (-) = (l, r) => gm2(l, r, Minus(u(l), u(r)));

let (-=) = (l, r) => l =@ l - r;

let (<) = (l, r) => g2(l, r, LessThan(u(l), u(r)));

let (>) = (l, r) => g2(l, r, GreaterThan(u(l), u(r)));

let (<=) = (l, r) => g2(l, r, LessThanOrEqual(u(l), u(r)));

let (>=) = (l, r) => g2(l, r, GreaterThanOrEqual(u(l), u(r)));

let (==) = (l, r) => g2(l, r, Equal(u(l), u(r)));

let (!=) = (l, r) => g2(l, r, NotEqual(u(l), u(r)));

let (&&) = (l, r) => g2(l, r, And(u(l), u(r)));

let (||) = (l, r) => g2(l, r, Or(u(l), u(r)));

let (^^) = (l, r) => g2(l, r, Xor(u(l), u(r)));

let ( **. ) = (var, st) => RSwizzle(var, st);

let ternary = (l, r1, r2) => Ternary(u(l), u(r1), u(r2));

let max = l => glist(l, BuiltinFun("max", List.map(u, l)));

let min = l => glist(l, BuiltinFun("min", List.map(u, l)));

let sqrt = l => g1(l, BuiltinFun1("sqrt", u(l)));

let sin = l => g1(l, BuiltinFun1("sin", u(l)));

let cos = l => g1(l, BuiltinFun1("cos", u(l)));

let abs = l => g1(l, BuiltinFun1("abs", u(l)));

let exp = l => g1(l, BuiltinFun1("exp", u(l)));

let length = l => genericexpr1float(l, BuiltinFun1("length", u(l)));

let floor = l => g1(l, BuiltinFun1("floor", u(l)));

let fract = l => g1(l, BuiltinFun1("fract", u(l)));

let pow = (l, r) => g2(l, r, BuiltinFun2("pow", u(l), u(r)));

let dot = (l, r) => genericexpr2float(l, r, BuiltinFun2("dot", u(l), u(r)));

let texture = (l, r) =>
  /* TODO: fix types as in commented line */
  Typed(`Vec4, BuiltinFun2("texture", u(l), u(r)));

/*switch (l, r) {
    /* | (Typed(`Sampler2D, _), Typed(`Vec2, _)) => Typed(`Vec4, Texture(l, r))*/
    | (Typed(`Sampler2D | `Vec2, _), Typed(`Sampler2D | `Vec2, _)) => (
      `Vec4,
      Texture(l, r)
    )
  };*/
let refract = (l, r1, r2) => {
  let e = BuiltinFun3("refract", u(l), u(r1), u(r2));
  switch (l, r1, r2) {
  | (Typed(`Float, _), Typed(`Float, _), Typed(`Float, _)) => Typed(`Float, e)
  | (Typed(`Vec2, _), Typed(`Vec2, _), Typed(`Float, _)) => Typed(`Vec2, e)
  | (Typed(`Vec3, _), Typed(`Vec3, _), Typed(`Float, _)) => Typed(`Vec3, e)
  | (Typed(`Vec4, _), Typed(`Vec4, _), Typed(`Float, _)) => Typed(`Vec4, e)
  | _ => raise(GLSLTypeError("type mismatch"))
  };
};

/*let vec2: cfun1 =*/
let vec2 = l => {
  let e = Typed(`Vec2, BuiltinFun("vec2", List.map(u, l)));
  switch l {
  | [Typed(`Float, _)] => e
  | [Typed(`Float, _), Typed(`Float, _)] => e
  | [Typed(`Vec2, _)] => e
  | _ => raise(GLSLTypeError("type mismatch"))
  };
};

let vec3 = l => {
  let ret = Typed(`Vec3, BuiltinFun("vec3", List.map(u, l)));
  switch l {
  | [Typed(`Float, _)] => ret
  | [Typed(`Float, _), Typed(`Float, _), Typed(`Float, _)] => ret
  | [Typed(`Vec2, _), Typed(`Float, _)] => ret
  | [Typed(`Vec3, _)] => ret
  | _ => raise(GLSLTypeError("type mismatch"))
  };
};

let vec4 = l => {
  let ret = Typed(`Vec4, BuiltinFun("vec4", List.map(u, l)));
  switch l {
  | [Typed(`Float, _)] => ret
  | [Typed(`Float, _), Typed(`Float, _), Typed(`Float, _), Typed(`Float, _)] => ret
  | [Typed(`Vec2, _), Typed(`Vec2, _)] => ret
  | [Typed(`Vec3, _), Typed(`Float, _)] => ret
  | [Typed(`Vec4, _)] => ret
  | _ => raise(GLSLTypeError("type mismatch"))
  };
};

/*
 let mat2 = l => BuiltinFun("mat2", l);

 let mat3 = l => BuiltinFun("mat3", l);

 let mat4 = l => BuiltinFun("mat4", l);
 */
let ifstmt = (l, b) => add(IfStatement(l, b));

let ifelsestmt = (l, b1, b2) => add(IfElseStatement(l, b1, b2));

/*let f: cfun1 = x => Typed(`Float, ImmediateFloat(x));*/
let f = x => Typed(`Float, ImmediateFloat(x));

let i = x => Typed(`Int, ImmediateInt(x));

let return = l => add(Return(l));

/* float var declaration and initialization */
let floatif = a => {
  let var = floatvar(genSym());
  var =@ f(a);
  var;
};

/* vec3 of 3 floats var declaration and initialization */
let vec3i3f = (a, b, c) => {
  let var = vec3var(genSym());
  var =@ vec3([f(a), f(b), f(c)]);
  var;
};

/* float of expr declaration and initialization */
let floati1 = a => {
  let var = floatvar(genSym());
  var =@ a;
  var;
};

/* vec3 of expr declaration and initialization */
let vec3i1 = a => {
  let var = vec3var(genSym());
  var =@ a;
  var;
};

/* TODO: Implement */
let const = x => x;

let nvec2init = (l, r) => vec2([f(l), f(r)]);

let i1 = nvec2init(0.0, 0.0);

let i2 = f(0.0);

let a = sin(i1);

let b = sin(i2);

let c = vec2var("c");

let tmp = vec2([b]);

c =@ vec2([b]);

c =@ tmp;
