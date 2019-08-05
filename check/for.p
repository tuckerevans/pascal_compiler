program main ( input, output );

        var a, b: integer;
        var x,y,z: real;
	var ai :array [1..10] of integer;
begin
{ TEST }

	a := 1;
        x := 3.14;
	b := a + 35;
	(* test *)
	for c := 0 to 10 do begin
		for a:= 10 downto 0 do b := a - c
	end
end.
