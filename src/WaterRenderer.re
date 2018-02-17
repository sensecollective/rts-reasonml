/*
 * WebGL Water
 * http://madebyevan.com/webgl-water/
 *
 * Copyright 2011 Evan Wallace
 * Ported to ReasonML by Eivind Magnus Hvidevold
 * Released under the MIT license
 */
open GLSL;

open! VertexShader;

let light = vec3uniform("light");

let sphereCenter = vec3uniform("sphereCenter");

let sphereRadius = floatuniform("sphereRadius");

let tiles = sampler2Duniform("tiles");

let causticTex = sampler2Duniform("causticTex");

let water = sampler2Duniform("water");

let helperFunctions = {
  let cIOR_AIR = const(floatif(1.0));
  let cIOR_WATER = const(floatif(1.333));
  let abovewaterColor = const(vec3i3f(0.25, 1.0, 1.25));
  let underwaterColor = const(vec3i3f(0.4, 0.9, 1.0));
  let poolHeight = const(floatif(1.0));
  let origin = vec3arg("origin");
  let ray = vec3arg("ray");
  let cubeMin = vec3arg("cubeMin");
  let cubeMax = vec3arg("cubeMax");
  let intersectCube =
    fundecl(
      vec2fun("intersectCube"),
      [origin, ray, cubeMin, cubeMax],
      {
        let tMin = vec3var("tMin");
        tMin =@ (cubeMin - origin) / ray;
        let tMax = vec3var("tMax");
        tMax =@ (cubeMax - origin) / ray;
        let t1 = vec3var("t1");
        t1 =@ min([tMin, tMax]);
        let t2 = vec3var("t2");
        t2 =@ max([tMin, tMax]);
        let tNear = floatvar("tNear");
        let tFar = floatvar("tFar");
        tNear =@ max([max([t1 **. X, t1 **. Y]), t1 **. Z]);
        tFar =@ min([min([t2 **. X, t2 **. Y]), t2 **. Z]);
        return(vec2([tNear, tFar]));
        finish();
      }
    );
  let sphereCenter = vec3arg("sphereCenter");
  let sphereRadius = floatarg("sphereRadius");
  let intersectSphere =
    fundecl(
      vec2fun("intersectSphere"),
      [origin, ray, sphereCenter, sphereRadius],
      {
        let toSphere = vec3i1(origin - sphereCenter);
        let a = floati1(dot(ray, ray));
        let b = floati1(f(2.0) * dot(toSphere, ray));
        let c = floati1(dot(toSphere, toSphere) - sphereRadius * sphereRadius);
        let discriminant = floatvar("discriminant");
        discriminant =@ b * b - f(4.0) * a * c;
        ifstmt(
          discriminant > f(0.0),
          {
            push();
            let t = floatvar("t");
            t =@ (f(0.0) - b - sqrt(discriminant)) / (f(2.0) * a);
            ifstmt(
              t > f(0.0),
              {
                push();
                return(t);
                pop();
              }
            );
            pop();
          }
        );
        finish();
      }
    );
  let point = vec3arg("point");
  let getSphereColor =
    fundecl(
      vec3fun("getSphereColor"),
      [point],
      {
        let color = vec3([f(0.5)]);
        /* ambient occlusion with walls */
        color
        *= (
          f(1.0)
          - f(0.9)
          / pow(
              (f(1.0) + sphereRadius - abs(point **. X)) / sphereRadius,
              f(3.0)
            )
        );
        color
        *= (
          f(1.0)
          - f(0.9)
          / pow(
              (f(1.0) + sphereRadius - abs(point **. Z)) / sphereRadius,
              f(3.0)
            )
        );
        color
        *= (
          f(1.0)
          - f(0.9)
          / pow((point **. Y + f(1.0) + sphereRadius) / sphereRadius, f(3.0))
        );
        /* caustics */
        let sphereNormal = vec3var("sphereNormal");
        sphereNormal =@ (point - sphereCenter) / sphereRadius;
        let refractedLight = vec3var("refractedLight");
        refractedLight
        =@ refract(
             f(0.0) - light,
             vec3([f(0.0), f(1.0), f(0.0)]),
             cIOR_AIR / cIOR_WATER
           );
        let diffuse = floatvar("diffuse");
        diffuse
        =@ max([f(0.0), dot(f(0.0) - refractedLight, sphereNormal)])
        * f(0.5);
        let info = vec4var("info");
        info =@ texture(water, point **. XZ * f(0.5) + f(0.5));
        ifstmt(
          point **. Y < info **. R,
          {
            push();
            let caustic = vec4var("caustic");
            caustic
            =@ texture(
                 causticTex,
                 f(0.75)
                 * (
                   point
                   **. XZ
                   - point
                   **. Y
                   * (refractedLight **. XZ)
                   / (refractedLight **. Y)
                 )
                 * f(0.5)
                 + f(0.5)
               );
            diffuse *= (caustic **. R * f(4.0));
            pop();
          }
        );
        color += diffuse;
        return(color);
        finish();
      }
    );
  let getWallColor =
    fundecl(
      vec3fun("getWallColor"),
      [point],
      {
        let scale = floatvar("scale");
        scale =@ f(0.5);
        let wallColor = vec3var("wallColor");
        wallColor =@ vec3([f(0.0)]);
        let normal = vec3var("normal");
        normal =@ vec3([f(0.0)]);
        ifelsestmt(
          abs(point **. X) > f(0.999),
          {
            push();
            wallColor
            =@ texture(tiles, point **. YZ * f(0.5) + vec2([f(1.0), f(0.5)]))
            **. RGB;
            normal =@ vec3([f(0.0) - point **. X, f(0.0), f(0.0)]);
            pop();
          },
          {
            push();
            ifelsestmt(
              abs(point **. Z) > f(0.999),
              {
                push();
                wallColor
                =@ texture(
                     tiles,
                     point **. YX * f(0.5) + vec2([f(1.0), f(0.5)])
                   )
                **. RGB;
                normal =@ vec3([f(0.0), f(0.0), f(0.0) - point **. Z]);
                pop();
              },
              {
                push();
                wallColor
                =@ texture(tiles, point **. XZ * f(0.5) + f(0.5))
                **. RGB;
                normal =@ vec3([f(0.0), f(1.0), f(0.0)]);
                pop();
              }
            );
            pop();
          }
        );
        scale /= length(point); /* pool ambient occlusion */
        scale
        *= (
          f(1.0)
          - f(0.9)
          / pow(length(point - sphereCenter) / sphereRadius, f(4.0))
        ); /* sphere ambient occlusion */
        /* caustics */
        let refractedLight = vec3var("refractedLight");
        refractedLight
        =@ f(0.0)
        - refract(
            f(0.0) - light,
            vec3([f(0.0), f(1.0), f(0.0)]),
            cIOR_AIR / cIOR_WATER
          );
        let diffuse = floatvar("diffuse");
        diffuse =@ max([f(0.0), dot(refractedLight, normal)]);
        let info = vec4var("info");
        info =@ texture(water, point **. XZ * f(0.5) + f(0.5));
        ifelsestmt(
          point **. Y < info **. R,
          {
            push();
            let caustic = vec4var("caustic");
            caustic
            =@ texture(
                 causticTex,
                 f(0.75)
                 * (
                   point
                   **. XZ
                   - point
                   **. Y
                   * (refractedLight **. XZ)
                   / (refractedLight **. Y)
                 )
                 * f(0.5)
                 + f(0.5)
               );
            scale += diffuse * (caustic **. R) * f(2.0) * (caustic **. G);
            pop();
          },
          {
            push();
            /* shadow for the rim of the pool */
            let t = vec2var("t");
            t
            =@ intersectCube([
                 point,
                 refractedLight,
                 vec3([f(-1.0), f(0.0) - poolHeight, f(-1.0)]),
                 vec3([f(1.0), f(2.0), f(1.0)])
               ]);
            diffuse
            *= (
              f(1.0)
              / (
                f(1.0)
                + exp(
                    f(-200.0)
                    / (f(1.0) + f(10.0) * (t **. Y - t **. X))
                    * (
                      point
                      **. Y
                      + refractedLight
                      **. Y
                      * (t **. Y)
                      - f(2.0)
                      / f(12.0)
                    )
                  )
              )
            );
            scale += diffuse * f(0.5);
            pop();
          }
        );
        return(wallColor * scale);
        finish();
      }
    );
  ();
};
/*
   }\
 ';

 function Renderer() {
   this.tileTexture = GL.Texture.fromImage(document.getElementById('tiles'), {
     minFilter: gl.LINEAR_MIPMAP_LINEAR,
     wrap: gl.REPEAT,
     format: gl.RGB
   });
   this.lightDir = new GL.Vector(2.0, 2.0, -1.0).unit();
   this.causticTex = new GL.Texture(1024, 1024);
   this.waterMesh = GL.Mesh.plane({ detail: 200 });
   this.waterShaders = [];
   for (var i = 0; i < 2; i++) {
     this.waterShaders[i] = new GL.Shader('\
       uniform sampler2D water;\
       varying vec3 position;\
       void main() {\
         vec4 info = texture2D(water, gl_Vertex.xy * 0.5 + 0.5);\
         position = gl_Vertex.xzy;\
         position.y += info.r;\
         gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);\
       }\
     ', helperFunctions + '\
       uniform vec3 eye;\
       varying vec3 position;\
       uniform samplerCube sky;\
       \
       vec3 getSurfaceRayColor(vec3 origin, vec3 ray, vec3 waterColor) {\
         vec3 color;\
         float q = intersectSphere(origin, ray, sphereCenter, sphereRadius);\
         if (q < 1.0e6) {\
           color = getSphereColor(origin + ray * q);\
         } else if (ray.y < 0.0) {\
           vec2 t = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));\
           color = getWallColor(origin + ray * t.y);\
         } else {\
           vec2 t = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));\
           vec3 hit = origin + ray * t.y;\
           if (hit.y < 2.0 / 12.0) {\
             color = getWallColor(hit);\
           } else {\
             color = textureCube(sky, ray).rgb;\
             color += vec3(pow(max(0.0, dot(light, ray)), 5000.0)) * vec3(10.0, 8.0, 6.0);\
           }\
         }\
         if (ray.y < 0.0) color *= waterColor;\
         return color;\
       }\
       \
       void main() {\
         vec2 coord = position.xz * 0.5 + 0.5;\
         vec4 info = texture2D(water, coord);\
         \
         /* make water look more "peaked" */\
         for (int i = 0; i < 5; i++) {\
           coord += info.ba * 0.005;\
           info = texture2D(water, coord);\
         }\
         \
         vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);\
         vec3 incomingRay = normalize(position - eye);\
         \
         ' + (i ? /* underwater */ '\
           normal = -normal;\
           vec3 reflectedRay = reflect(incomingRay, normal);\
           vec3 refractedRay = refract(incomingRay, normal, IOR_WATER / IOR_AIR);\
           float fresnel = mix(0.5, 1.0, pow(1.0 - dot(normal, -incomingRay), 3.0));\
           \
           vec3 reflectedColor = getSurfaceRayColor(position, reflectedRay, underwaterColor);\
           vec3 refractedColor = getSurfaceRayColor(position, refractedRay, vec3(1.0)) * vec3(0.8, 1.0, 1.1);\
           \
           gl_FragColor = vec4(mix(reflectedColor, refractedColor, (1.0 - fresnel) * length(refractedRay)), 1.0);\
         ' : /* above water */ '\
           vec3 reflectedRay = reflect(incomingRay, normal);\
           vec3 refractedRay = refract(incomingRay, normal, IOR_AIR / IOR_WATER);\
           float fresnel = mix(0.25, 1.0, pow(1.0 - dot(normal, -incomingRay), 3.0));\
           \
           vec3 reflectedColor = getSurfaceRayColor(position, reflectedRay, abovewaterColor);\
           vec3 refractedColor = getSurfaceRayColor(position, refractedRay, abovewaterColor);\
           \
           gl_FragColor = vec4(mix(refractedColor, reflectedColor, fresnel), 1.0);\
         ') + '\
       }\
     ');
   }
   this.sphereMesh = GL.Mesh.sphere({ detail: 10 });
   this.sphereShader = new GL.Shader(helperFunctions + '\
     varying vec3 position;\
     void main() {\
       position = sphereCenter + gl_Vertex.xyz * sphereRadius;\
       gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);\
     }\
   ', helperFunctions + '\
     varying vec3 position;\
     void main() {\
       gl_FragColor = vec4(getSphereColor(position), 1.0);\
       vec4 info = texture2D(water, position.xz * 0.5 + 0.5);\
       if (position.y < info.r) {\
         gl_FragColor.rgb *= underwaterColor * 1.2;\
       }\
     }\
   ');
   this.cubeMesh = GL.Mesh.cube();
   this.cubeMesh.triangles.splice(4, 2);
   this.cubeMesh.compile();
   this.cubeShader = new GL.Shader(helperFunctions + '\
     varying vec3 position;\
     void main() {\
       position = gl_Vertex.xyz;\
       position.y = ((1.0 - position.y) * (7.0 / 12.0) - 1.0) * poolHeight;\
       gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);\
     }\
   ', helperFunctions + '\
     varying vec3 position;\
     void main() {\
       gl_FragColor = vec4(getWallColor(position), 1.0);\
       vec4 info = texture2D(water, position.xz * 0.5 + 0.5);\
       if (position.y < info.r) {\
         gl_FragColor.rgb *= underwaterColor * 1.2;\
       }\
     }\
   ');
   this.sphereCenter = new GL.Vector();
   this.sphereRadius = 0;
   var hasDerivatives = !!gl.getExtension('OES_standard_derivatives');
   this.causticsShader = new GL.Shader(helperFunctions + '\
     varying vec3 oldPos;\
     varying vec3 newPos;\
     varying vec3 ray;\
     \
     /* project the ray onto the plane */\
     vec3 project(vec3 origin, vec3 ray, vec3 refractedLight) {\
       vec2 tcube = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));\
       origin += ray * tcube.y;\
       float tplane = (-origin.y - 1.0) / refractedLight.y;\
       return origin + refractedLight * tplane;\
     }\
     \
     void main() {\
       vec4 info = texture2D(water, gl_Vertex.xy * 0.5 + 0.5);\
       info.ba *= 0.5;\
       vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);\
       \
       /* project the vertices along the refracted vertex ray */\
       vec3 refractedLight = refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);\
       ray = refract(-light, normal, IOR_AIR / IOR_WATER);\
       oldPos = project(gl_Vertex.xzy, refractedLight, refractedLight);\
       newPos = project(gl_Vertex.xzy + vec3(0.0, info.r, 0.0), ray, refractedLight);\
       \
       gl_Position = vec4(0.75 * (newPos.xz + refractedLight.xz / refractedLight.y), 0.0, 1.0);\
     }\
   ', (hasDerivatives ? '#extension GL_OES_standard_derivatives : enable\n' : '') + '\
     ' + helperFunctions + '\
     varying vec3 oldPos;\
     varying vec3 newPos;\
     varying vec3 ray;\
     \
     void main() {\
       ' + (hasDerivatives ? '\
         /* if the triangle gets smaller, it gets brighter, and vice versa */\
         float oldArea = length(dFdx(oldPos)) * length(dFdy(oldPos));\
         float newArea = length(dFdx(newPos)) * length(dFdy(newPos));\
         gl_FragColor = vec4(oldArea / newArea * 0.2, 1.0, 0.0, 0.0);\
       ' : '\
         gl_FragColor = vec4(0.2, 0.2, 0.0, 0.0);\
       ' ) + '\
       \
       vec3 refractedLight = refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);\
       \
       /* compute a blob shadow and make sure we only draw a shadow if the player is blocking the light */\
       vec3 dir = (sphereCenter - newPos) / sphereRadius;\
       vec3 area = cross(dir, refractedLight);\
       float shadow = dot(area, area);\
       float dist = dot(dir, -refractedLight);\
       shadow = 1.0 + (shadow - 1.0) / (0.05 + dist * 0.025);\
       shadow = clamp(1.0 / (1.0 + exp(-shadow)), 0.0, 1.0);\
       shadow = mix(1.0, shadow, clamp(dist * 2.0, 0.0, 1.0));\
       gl_FragColor.g = shadow;\
       \
       /* shadow for the rim of the pool */\
       vec2 t = intersectCube(newPos, -refractedLight, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));\
       gl_FragColor.r *= 1.0 / (1.0 + exp(-200.0 / (1.0 + 10.0 * (t.y - t.x)) * (newPos.y - refractedLight.y * t.y - 2.0 / 12.0)));\
     }\
   ');
 }

 Renderer.prototype.updateCaustics = function(water) {
   if (!this.causticsShader) return;
   var this_ = this;
   this.causticTex.drawTo(function() {
     gl.clear(gl.COLOR_BUFFER_BIT);
     water.textureA.bind(0);
     this_.causticsShader.uniforms({
       light: this_.lightDir,
       water: 0,
       sphereCenter: this_.sphereCenter,
       sphereRadius: this_.sphereRadius
     }).draw(this_.waterMesh);
   });
 };

 Renderer.prototype.renderWater = function(water, sky) {
   var tracer = new GL.Raytracer();
   water.textureA.bind(0);
   this.tileTexture.bind(1);
   sky.bind(2);
   this.causticTex.bind(3);
   gl.enable(gl.CULL_FACE);
   for (var i = 0; i < 2; i++) {
     gl.cullFace(i ? gl.BACK : gl.FRONT);
     this.waterShaders[i].uniforms({
       light: this.lightDir,
       water: 0,
       tiles: 1,
       sky: 2,
       causticTex: 3,
       eye: tracer.eye,
       sphereCenter: this.sphereCenter,
       sphereRadius: this.sphereRadius
     }).draw(this.waterMesh);
   }
   gl.disable(gl.CULL_FACE);
 };

 Renderer.prototype.renderSphere = function() {
   water.textureA.bind(0);
   this.causticTex.bind(1);
   this.sphereShader.uniforms({
     light: this.lightDir,
     water: 0,
     causticTex: 1,
     sphereCenter: this.sphereCenter,
     sphereRadius: this.sphereRadius
   }).draw(this.sphereMesh);
 };

 Renderer.prototype.renderCube = function() {
   gl.enable(gl.CULL_FACE);
   water.textureA.bind(0);
   this.tileTexture.bind(1);
   this.causticTex.bind(2);
   this.cubeShader.uniforms({
     light: this.lightDir,
     water: 0,
     tiles: 1,
     causticTex: 2,
     sphereCenter: this.sphereCenter,
     sphereRadius: this.sphereRadius
   }).draw(this.cubeMesh);
   gl.disable(gl.CULL_FACE);
 };
 */
