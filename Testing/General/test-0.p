program main(input);
 var a: integer;
 var b: array[1 .. 10] of real;
 var x: real;
 function foo(x: integer; y: real):integer;
 begin
	foo := foo(x-1) + 1;
	x := 13
 end;
begin
 if ( x > 3.14 and x < 0.0 ) then
 	x := 1.0
 else
	x := 0.0
end.












