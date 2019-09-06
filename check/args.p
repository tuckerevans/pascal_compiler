program main ( input, output );

        var a, b: integer;
        var x,y,z: real;
	var ai :array [1..10] of integer;

	function bar (a, b: integer) : real;
	var test:integer;
	begin
		test := 2;
		a := 2
	end;
	function bar1 (a, b: real) : real;
	var test:integer;
	begin
		test := 2;
		a := 2.0
	end;
	function bar2 (a: real) : real;
	var test:integer;
	begin
		test := 2;
		a := 2.0
	end;
	function bar3 (a, b, c, d, e, f, g: integer) : real;
	var test:integer;
	begin
		test := 2;
		a := 2
	end;
	function bar4 (a, b, c, d: real; e, f, g: integer) : real;
	var test:integer;
	begin
		test := 2;
		a := 2.0;
		e := 2
	end;
begin
{ TEST }

	a := 1;
        x := 3.14;
	b := a + 35
	(* test *)
end.
