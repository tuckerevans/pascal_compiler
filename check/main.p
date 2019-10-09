program main ( input, output );

        var a, b, c, d, e, f: integer;
        var x,y,z: real;
	var ai :array [1..10] of integer;
	procedure boo (n: integer);
	var a,c: integer;
	begin
		a := n
	end;
	function bar (a: integer) : real;
	var test:integer;
	begin
		test := 2;
		a := 2;
		bar := 2.1
	end;
	procedure foo;
	begin
		a := 33333;
		x := 1e10;
		z := 1e-10;
		y := 2.5543e-2
	end;
	function order (a, b, c: integer) : real;
	begin
		a := 2;
		order := 2.1

	end;
	function order2 (a, b, c, d, e: integer; f, g, h, i,j :real) : real;
	begin
		a := 2;
		order2 := 2.1
	end;
begin
{ TEST }

	a := 1;
        x := 3.14;
	x := order(a, b, c);
	x := order2(a, b, c, d, e, 1.0, 1.0, 1.0, 1.0, 1.0);
	x := bar(a);
	foo;
	boo(a);
	b := a + 35
	(* test *)
end.
