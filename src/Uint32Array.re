type t;

[@bs.new] external create : array(float) => t = "Uint32Array";

[@bs.new] external createSize : int => t = "Uint32Array";

[@bs.get] external length : t => int = "length";

[@bs.get_index] external get : (t, int) => int = "";

[@bs.set_index] external set : (t, int, int) => unit = "";
