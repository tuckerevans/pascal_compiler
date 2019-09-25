(* LEGAL: local names should hide non-local names *)
program main( input, output );
  var a,b: integer;
  procedure boo(a: integer);
    var b: real;
  begin
    b := 1
  end;
begin
  boo(b)
end.

