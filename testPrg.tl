-- Program 1: Vypocet faktorialu (iterativne)
require "ifj21"

global foo : function (integer) : integer

function main() -- uzivatelska funkce bez parametru
  local b : string
  local a : integer = foo(b)

  local vysl : string = 0
  vysl = #("ahoj svete" .. "a") + a
  write("Zadejte cislo pro vypocet faktorialu\n")
  a = readi()
  if a == nil then
      write("a je nil\n") return
  else
  end
  if a < 0 then
    write("Faktorial nelze spocitat\n")
  else
    vysl = 1
    while a > 0 do
       a = a - 1  -- dva prikazy
    end
    write("Vysledek je: ", vysl, "\n")
  end
end

main()  -- prikaz hlavniho tela programu