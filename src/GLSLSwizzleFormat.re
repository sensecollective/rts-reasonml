/* Auto-generated by GLSLGenerator.py */
include GLSLSwizzleType;

let fmtSwizzle = (swizzle: swizzleT) =>
  switch swizzle {
    | A => "a" 
  | AA => "aa" 
  | AAA => "aaa" 
  | AAAA => "aaaa" 
  | AAAB => "aaab" 
  | AAAG => "aaag" 
  | AAAR => "aaar" 
  | AAB => "aab" 
  | AABA => "aaba" 
  | AABB => "aabb" 
  | AABG => "aabg" 
  | AABR => "aabr" 
  | AAG => "aag" 
  | AAGA => "aaga" 
  | AAGB => "aagb" 
  | AAGG => "aagg" 
  | AAGR => "aagr" 
  | AAR => "aar" 
  | AARA => "aara" 
  | AARB => "aarb" 
  | AARG => "aarg" 
  | AARR => "aarr" 
  | AB => "ab" 
  | ABA => "aba" 
  | ABAA => "abaa" 
  | ABAB => "abab" 
  | ABAG => "abag" 
  | ABAR => "abar" 
  | ABB => "abb" 
  | ABBA => "abba" 
  | ABBB => "abbb" 
  | ABBG => "abbg" 
  | ABBR => "abbr" 
  | ABG => "abg" 
  | ABGA => "abga" 
  | ABGB => "abgb" 
  | ABGG => "abgg" 
  | ABGR => "abgr" 
  | ABR => "abr" 
  | ABRA => "abra" 
  | ABRB => "abrb" 
  | ABRG => "abrg" 
  | ABRR => "abrr" 
  | AG => "ag" 
  | AGA => "aga" 
  | AGAA => "agaa" 
  | AGAB => "agab" 
  | AGAG => "agag" 
  | AGAR => "agar" 
  | AGB => "agb" 
  | AGBA => "agba" 
  | AGBB => "agbb" 
  | AGBG => "agbg" 
  | AGBR => "agbr" 
  | AGG => "agg" 
  | AGGA => "agga" 
  | AGGB => "aggb" 
  | AGGG => "aggg" 
  | AGGR => "aggr" 
  | AGR => "agr" 
  | AGRA => "agra" 
  | AGRB => "agrb" 
  | AGRG => "agrg" 
  | AGRR => "agrr" 
  | AR => "ar" 
  | ARA => "ara" 
  | ARAA => "araa" 
  | ARAB => "arab" 
  | ARAG => "arag" 
  | ARAR => "arar" 
  | ARB => "arb" 
  | ARBA => "arba" 
  | ARBB => "arbb" 
  | ARBG => "arbg" 
  | ARBR => "arbr" 
  | ARG => "arg" 
  | ARGA => "arga" 
  | ARGB => "argb" 
  | ARGG => "argg" 
  | ARGR => "argr" 
  | ARR => "arr" 
  | ARRA => "arra" 
  | ARRB => "arrb" 
  | ARRG => "arrg" 
  | ARRR => "arrr" 
  | B => "b" 
  | BA => "ba" 
  | BAA => "baa" 
  | BAAA => "baaa" 
  | BAAB => "baab" 
  | BAAG => "baag" 
  | BAAR => "baar" 
  | BAB => "bab" 
  | BABA => "baba" 
  | BABB => "babb" 
  | BABG => "babg" 
  | BABR => "babr" 
  | BAG => "bag" 
  | BAGA => "baga" 
  | BAGB => "bagb" 
  | BAGG => "bagg" 
  | BAGR => "bagr" 
  | BAR => "bar" 
  | BARA => "bara" 
  | BARB => "barb" 
  | BARG => "barg" 
  | BARR => "barr" 
  | BB => "bb" 
  | BBA => "bba" 
  | BBAA => "bbaa" 
  | BBAB => "bbab" 
  | BBAG => "bbag" 
  | BBAR => "bbar" 
  | BBB => "bbb" 
  | BBBA => "bbba" 
  | BBBB => "bbbb" 
  | BBBG => "bbbg" 
  | BBBR => "bbbr" 
  | BBG => "bbg" 
  | BBGA => "bbga" 
  | BBGB => "bbgb" 
  | BBGG => "bbgg" 
  | BBGR => "bbgr" 
  | BBR => "bbr" 
  | BBRA => "bbra" 
  | BBRB => "bbrb" 
  | BBRG => "bbrg" 
  | BBRR => "bbrr" 
  | BG => "bg" 
  | BGA => "bga" 
  | BGAA => "bgaa" 
  | BGAB => "bgab" 
  | BGAG => "bgag" 
  | BGAR => "bgar" 
  | BGB => "bgb" 
  | BGBA => "bgba" 
  | BGBB => "bgbb" 
  | BGBG => "bgbg" 
  | BGBR => "bgbr" 
  | BGG => "bgg" 
  | BGGA => "bgga" 
  | BGGB => "bggb" 
  | BGGG => "bggg" 
  | BGGR => "bggr" 
  | BGR => "bgr" 
  | BGRA => "bgra" 
  | BGRB => "bgrb" 
  | BGRG => "bgrg" 
  | BGRR => "bgrr" 
  | BR => "br" 
  | BRA => "bra" 
  | BRAA => "braa" 
  | BRAB => "brab" 
  | BRAG => "brag" 
  | BRAR => "brar" 
  | BRB => "brb" 
  | BRBA => "brba" 
  | BRBB => "brbb" 
  | BRBG => "brbg" 
  | BRBR => "brbr" 
  | BRG => "brg" 
  | BRGA => "brga" 
  | BRGB => "brgb" 
  | BRGG => "brgg" 
  | BRGR => "brgr" 
  | BRR => "brr" 
  | BRRA => "brra" 
  | BRRB => "brrb" 
  | BRRG => "brrg" 
  | BRRR => "brrr" 
  | G => "g" 
  | GA => "ga" 
  | GAA => "gaa" 
  | GAAA => "gaaa" 
  | GAAB => "gaab" 
  | GAAG => "gaag" 
  | GAAR => "gaar" 
  | GAB => "gab" 
  | GABA => "gaba" 
  | GABB => "gabb" 
  | GABG => "gabg" 
  | GABR => "gabr" 
  | GAG => "gag" 
  | GAGA => "gaga" 
  | GAGB => "gagb" 
  | GAGG => "gagg" 
  | GAGR => "gagr" 
  | GAR => "gar" 
  | GARA => "gara" 
  | GARB => "garb" 
  | GARG => "garg" 
  | GARR => "garr" 
  | GB => "gb" 
  | GBA => "gba" 
  | GBAA => "gbaa" 
  | GBAB => "gbab" 
  | GBAG => "gbag" 
  | GBAR => "gbar" 
  | GBB => "gbb" 
  | GBBA => "gbba" 
  | GBBB => "gbbb" 
  | GBBG => "gbbg" 
  | GBBR => "gbbr" 
  | GBG => "gbg" 
  | GBGA => "gbga" 
  | GBGB => "gbgb" 
  | GBGG => "gbgg" 
  | GBGR => "gbgr" 
  | GBR => "gbr" 
  | GBRA => "gbra" 
  | GBRB => "gbrb" 
  | GBRG => "gbrg" 
  | GBRR => "gbrr" 
  | GG => "gg" 
  | GGA => "gga" 
  | GGAA => "ggaa" 
  | GGAB => "ggab" 
  | GGAG => "ggag" 
  | GGAR => "ggar" 
  | GGB => "ggb" 
  | GGBA => "ggba" 
  | GGBB => "ggbb" 
  | GGBG => "ggbg" 
  | GGBR => "ggbr" 
  | GGG => "ggg" 
  | GGGA => "ggga" 
  | GGGB => "gggb" 
  | GGGG => "gggg" 
  | GGGR => "gggr" 
  | GGR => "ggr" 
  | GGRA => "ggra" 
  | GGRB => "ggrb" 
  | GGRG => "ggrg" 
  | GGRR => "ggrr" 
  | GR => "gr" 
  | GRA => "gra" 
  | GRAA => "graa" 
  | GRAB => "grab" 
  | GRAG => "grag" 
  | GRAR => "grar" 
  | GRB => "grb" 
  | GRBA => "grba" 
  | GRBB => "grbb" 
  | GRBG => "grbg" 
  | GRBR => "grbr" 
  | GRG => "grg" 
  | GRGA => "grga" 
  | GRGB => "grgb" 
  | GRGG => "grgg" 
  | GRGR => "grgr" 
  | GRR => "grr" 
  | GRRA => "grra" 
  | GRRB => "grrb" 
  | GRRG => "grrg" 
  | GRRR => "grrr" 
  | R => "r" 
  | RA => "ra" 
  | RAA => "raa" 
  | RAAA => "raaa" 
  | RAAB => "raab" 
  | RAAG => "raag" 
  | RAAR => "raar" 
  | RAB => "rab" 
  | RABA => "raba" 
  | RABB => "rabb" 
  | RABG => "rabg" 
  | RABR => "rabr" 
  | RAG => "rag" 
  | RAGA => "raga" 
  | RAGB => "ragb" 
  | RAGG => "ragg" 
  | RAGR => "ragr" 
  | RAR => "rar" 
  | RARA => "rara" 
  | RARB => "rarb" 
  | RARG => "rarg" 
  | RARR => "rarr" 
  | RB => "rb" 
  | RBA => "rba" 
  | RBAA => "rbaa" 
  | RBAB => "rbab" 
  | RBAG => "rbag" 
  | RBAR => "rbar" 
  | RBB => "rbb" 
  | RBBA => "rbba" 
  | RBBB => "rbbb" 
  | RBBG => "rbbg" 
  | RBBR => "rbbr" 
  | RBG => "rbg" 
  | RBGA => "rbga" 
  | RBGB => "rbgb" 
  | RBGG => "rbgg" 
  | RBGR => "rbgr" 
  | RBR => "rbr" 
  | RBRA => "rbra" 
  | RBRB => "rbrb" 
  | RBRG => "rbrg" 
  | RBRR => "rbrr" 
  | RG => "rg" 
  | RGA => "rga" 
  | RGAA => "rgaa" 
  | RGAB => "rgab" 
  | RGAG => "rgag" 
  | RGAR => "rgar" 
  | RGB => "rgb" 
  | RGBA => "rgba" 
  | RGBB => "rgbb" 
  | RGBG => "rgbg" 
  | RGBR => "rgbr" 
  | RGG => "rgg" 
  | RGGA => "rgga" 
  | RGGB => "rggb" 
  | RGGG => "rggg" 
  | RGGR => "rggr" 
  | RGR => "rgr" 
  | RGRA => "rgra" 
  | RGRB => "rgrb" 
  | RGRG => "rgrg" 
  | RGRR => "rgrr" 
  | RR => "rr" 
  | RRA => "rra" 
  | RRAA => "rraa" 
  | RRAB => "rrab" 
  | RRAG => "rrag" 
  | RRAR => "rrar" 
  | RRB => "rrb" 
  | RRBA => "rrba" 
  | RRBB => "rrbb" 
  | RRBG => "rrbg" 
  | RRBR => "rrbr" 
  | RRG => "rrg" 
  | RRGA => "rrga" 
  | RRGB => "rrgb" 
  | RRGG => "rrgg" 
  | RRGR => "rrgr" 
  | RRR => "rrr" 
  | RRRA => "rrra" 
  | RRRB => "rrrb" 
  | RRRG => "rrrg" 
  | RRRR => "rrrr" 
  | W => "w" 
  | WW => "ww" 
  | WWW => "www" 
  | WWWW => "wwww" 
  | WWWX => "wwwx" 
  | WWWY => "wwwy" 
  | WWWZ => "wwwz" 
  | WWX => "wwx" 
  | WWXW => "wwxw" 
  | WWXX => "wwxx" 
  | WWXY => "wwxy" 
  | WWXZ => "wwxz" 
  | WWY => "wwy" 
  | WWYW => "wwyw" 
  | WWYX => "wwyx" 
  | WWYY => "wwyy" 
  | WWYZ => "wwyz" 
  | WWZ => "wwz" 
  | WWZW => "wwzw" 
  | WWZX => "wwzx" 
  | WWZY => "wwzy" 
  | WWZZ => "wwzz" 
  | WX => "wx" 
  | WXW => "wxw" 
  | WXWW => "wxww" 
  | WXWX => "wxwx" 
  | WXWY => "wxwy" 
  | WXWZ => "wxwz" 
  | WXX => "wxx" 
  | WXXW => "wxxw" 
  | WXXX => "wxxx" 
  | WXXY => "wxxy" 
  | WXXZ => "wxxz" 
  | WXY => "wxy" 
  | WXYW => "wxyw" 
  | WXYX => "wxyx" 
  | WXYY => "wxyy" 
  | WXYZ => "wxyz" 
  | WXZ => "wxz" 
  | WXZW => "wxzw" 
  | WXZX => "wxzx" 
  | WXZY => "wxzy" 
  | WXZZ => "wxzz" 
  | WY => "wy" 
  | WYW => "wyw" 
  | WYWW => "wyww" 
  | WYWX => "wywx" 
  | WYWY => "wywy" 
  | WYWZ => "wywz" 
  | WYX => "wyx" 
  | WYXW => "wyxw" 
  | WYXX => "wyxx" 
  | WYXY => "wyxy" 
  | WYXZ => "wyxz" 
  | WYY => "wyy" 
  | WYYW => "wyyw" 
  | WYYX => "wyyx" 
  | WYYY => "wyyy" 
  | WYYZ => "wyyz" 
  | WYZ => "wyz" 
  | WYZW => "wyzw" 
  | WYZX => "wyzx" 
  | WYZY => "wyzy" 
  | WYZZ => "wyzz" 
  | WZ => "wz" 
  | WZW => "wzw" 
  | WZWW => "wzww" 
  | WZWX => "wzwx" 
  | WZWY => "wzwy" 
  | WZWZ => "wzwz" 
  | WZX => "wzx" 
  | WZXW => "wzxw" 
  | WZXX => "wzxx" 
  | WZXY => "wzxy" 
  | WZXZ => "wzxz" 
  | WZY => "wzy" 
  | WZYW => "wzyw" 
  | WZYX => "wzyx" 
  | WZYY => "wzyy" 
  | WZYZ => "wzyz" 
  | WZZ => "wzz" 
  | WZZW => "wzzw" 
  | WZZX => "wzzx" 
  | WZZY => "wzzy" 
  | WZZZ => "wzzz" 
  | X => "x" 
  | XW => "xw" 
  | XWW => "xww" 
  | XWWW => "xwww" 
  | XWWX => "xwwx" 
  | XWWY => "xwwy" 
  | XWWZ => "xwwz" 
  | XWX => "xwx" 
  | XWXW => "xwxw" 
  | XWXX => "xwxx" 
  | XWXY => "xwxy" 
  | XWXZ => "xwxz" 
  | XWY => "xwy" 
  | XWYW => "xwyw" 
  | XWYX => "xwyx" 
  | XWYY => "xwyy" 
  | XWYZ => "xwyz" 
  | XWZ => "xwz" 
  | XWZW => "xwzw" 
  | XWZX => "xwzx" 
  | XWZY => "xwzy" 
  | XWZZ => "xwzz" 
  | XX => "xx" 
  | XXW => "xxw" 
  | XXWW => "xxww" 
  | XXWX => "xxwx" 
  | XXWY => "xxwy" 
  | XXWZ => "xxwz" 
  | XXX => "xxx" 
  | XXXW => "xxxw" 
  | XXXX => "xxxx" 
  | XXXY => "xxxy" 
  | XXXZ => "xxxz" 
  | XXY => "xxy" 
  | XXYW => "xxyw" 
  | XXYX => "xxyx" 
  | XXYY => "xxyy" 
  | XXYZ => "xxyz" 
  | XXZ => "xxz" 
  | XXZW => "xxzw" 
  | XXZX => "xxzx" 
  | XXZY => "xxzy" 
  | XXZZ => "xxzz" 
  | XY => "xy" 
  | XYW => "xyw" 
  | XYWW => "xyww" 
  | XYWX => "xywx" 
  | XYWY => "xywy" 
  | XYWZ => "xywz" 
  | XYX => "xyx" 
  | XYXW => "xyxw" 
  | XYXX => "xyxx" 
  | XYXY => "xyxy" 
  | XYXZ => "xyxz" 
  | XYY => "xyy" 
  | XYYW => "xyyw" 
  | XYYX => "xyyx" 
  | XYYY => "xyyy" 
  | XYYZ => "xyyz" 
  | XYZ => "xyz" 
  | XYZW => "xyzw" 
  | XYZX => "xyzx" 
  | XYZY => "xyzy" 
  | XYZZ => "xyzz" 
  | XZ => "xz" 
  | XZW => "xzw" 
  | XZWW => "xzww" 
  | XZWX => "xzwx" 
  | XZWY => "xzwy" 
  | XZWZ => "xzwz" 
  | XZX => "xzx" 
  | XZXW => "xzxw" 
  | XZXX => "xzxx" 
  | XZXY => "xzxy" 
  | XZXZ => "xzxz" 
  | XZY => "xzy" 
  | XZYW => "xzyw" 
  | XZYX => "xzyx" 
  | XZYY => "xzyy" 
  | XZYZ => "xzyz" 
  | XZZ => "xzz" 
  | XZZW => "xzzw" 
  | XZZX => "xzzx" 
  | XZZY => "xzzy" 
  | XZZZ => "xzzz" 
  | Y => "y" 
  | YW => "yw" 
  | YWW => "yww" 
  | YWWW => "ywww" 
  | YWWX => "ywwx" 
  | YWWY => "ywwy" 
  | YWWZ => "ywwz" 
  | YWX => "ywx" 
  | YWXW => "ywxw" 
  | YWXX => "ywxx" 
  | YWXY => "ywxy" 
  | YWXZ => "ywxz" 
  | YWY => "ywy" 
  | YWYW => "ywyw" 
  | YWYX => "ywyx" 
  | YWYY => "ywyy" 
  | YWYZ => "ywyz" 
  | YWZ => "ywz" 
  | YWZW => "ywzw" 
  | YWZX => "ywzx" 
  | YWZY => "ywzy" 
  | YWZZ => "ywzz" 
  | YX => "yx" 
  | YXW => "yxw" 
  | YXWW => "yxww" 
  | YXWX => "yxwx" 
  | YXWY => "yxwy" 
  | YXWZ => "yxwz" 
  | YXX => "yxx" 
  | YXXW => "yxxw" 
  | YXXX => "yxxx" 
  | YXXY => "yxxy" 
  | YXXZ => "yxxz" 
  | YXY => "yxy" 
  | YXYW => "yxyw" 
  | YXYX => "yxyx" 
  | YXYY => "yxyy" 
  | YXYZ => "yxyz" 
  | YXZ => "yxz" 
  | YXZW => "yxzw" 
  | YXZX => "yxzx" 
  | YXZY => "yxzy" 
  | YXZZ => "yxzz" 
  | YY => "yy" 
  | YYW => "yyw" 
  | YYWW => "yyww" 
  | YYWX => "yywx" 
  | YYWY => "yywy" 
  | YYWZ => "yywz" 
  | YYX => "yyx" 
  | YYXW => "yyxw" 
  | YYXX => "yyxx" 
  | YYXY => "yyxy" 
  | YYXZ => "yyxz" 
  | YYY => "yyy" 
  | YYYW => "yyyw" 
  | YYYX => "yyyx" 
  | YYYY => "yyyy" 
  | YYYZ => "yyyz" 
  | YYZ => "yyz" 
  | YYZW => "yyzw" 
  | YYZX => "yyzx" 
  | YYZY => "yyzy" 
  | YYZZ => "yyzz" 
  | YZ => "yz" 
  | YZW => "yzw" 
  | YZWW => "yzww" 
  | YZWX => "yzwx" 
  | YZWY => "yzwy" 
  | YZWZ => "yzwz" 
  | YZX => "yzx" 
  | YZXW => "yzxw" 
  | YZXX => "yzxx" 
  | YZXY => "yzxy" 
  | YZXZ => "yzxz" 
  | YZY => "yzy" 
  | YZYW => "yzyw" 
  | YZYX => "yzyx" 
  | YZYY => "yzyy" 
  | YZYZ => "yzyz" 
  | YZZ => "yzz" 
  | YZZW => "yzzw" 
  | YZZX => "yzzx" 
  | YZZY => "yzzy" 
  | YZZZ => "yzzz" 
  | Z => "z" 
  | ZW => "zw" 
  | ZWW => "zww" 
  | ZWWW => "zwww" 
  | ZWWX => "zwwx" 
  | ZWWY => "zwwy" 
  | ZWWZ => "zwwz" 
  | ZWX => "zwx" 
  | ZWXW => "zwxw" 
  | ZWXX => "zwxx" 
  | ZWXY => "zwxy" 
  | ZWXZ => "zwxz" 
  | ZWY => "zwy" 
  | ZWYW => "zwyw" 
  | ZWYX => "zwyx" 
  | ZWYY => "zwyy" 
  | ZWYZ => "zwyz" 
  | ZWZ => "zwz" 
  | ZWZW => "zwzw" 
  | ZWZX => "zwzx" 
  | ZWZY => "zwzy" 
  | ZWZZ => "zwzz" 
  | ZX => "zx" 
  | ZXW => "zxw" 
  | ZXWW => "zxww" 
  | ZXWX => "zxwx" 
  | ZXWY => "zxwy" 
  | ZXWZ => "zxwz" 
  | ZXX => "zxx" 
  | ZXXW => "zxxw" 
  | ZXXX => "zxxx" 
  | ZXXY => "zxxy" 
  | ZXXZ => "zxxz" 
  | ZXY => "zxy" 
  | ZXYW => "zxyw" 
  | ZXYX => "zxyx" 
  | ZXYY => "zxyy" 
  | ZXYZ => "zxyz" 
  | ZXZ => "zxz" 
  | ZXZW => "zxzw" 
  | ZXZX => "zxzx" 
  | ZXZY => "zxzy" 
  | ZXZZ => "zxzz" 
  | ZY => "zy" 
  | ZYW => "zyw" 
  | ZYWW => "zyww" 
  | ZYWX => "zywx" 
  | ZYWY => "zywy" 
  | ZYWZ => "zywz" 
  | ZYX => "zyx" 
  | ZYXW => "zyxw" 
  | ZYXX => "zyxx" 
  | ZYXY => "zyxy" 
  | ZYXZ => "zyxz" 
  | ZYY => "zyy" 
  | ZYYW => "zyyw" 
  | ZYYX => "zyyx" 
  | ZYYY => "zyyy" 
  | ZYYZ => "zyyz" 
  | ZYZ => "zyz" 
  | ZYZW => "zyzw" 
  | ZYZX => "zyzx" 
  | ZYZY => "zyzy" 
  | ZYZZ => "zyzz" 
  | ZZ => "zz" 
  | ZZW => "zzw" 
  | ZZWW => "zzww" 
  | ZZWX => "zzwx" 
  | ZZWY => "zzwy" 
  | ZZWZ => "zzwz" 
  | ZZX => "zzx" 
  | ZZXW => "zzxw" 
  | ZZXX => "zzxx" 
  | ZZXY => "zzxy" 
  | ZZXZ => "zzxz" 
  | ZZY => "zzy" 
  | ZZYW => "zzyw" 
  | ZZYX => "zzyx" 
  | ZZYY => "zzyy" 
  | ZZYZ => "zzyz" 
  | ZZZ => "zzz" 
  | ZZZW => "zzzw" 
  | ZZZX => "zzzx" 
  | ZZZY => "zzzy" 
  | ZZZZ => "zzzz" 
  };
