program main ( input, output );
        var a: integer;
	function bar (b: integer) : integer;
	var test:integer;
	begin
		test := bar(test);
		bar := test
	end;
begin
	a := bar(1);
	a := 1 + 1
end.
