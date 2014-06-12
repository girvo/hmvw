<?hh

class MyClass 
{
    public function my_negation(bool $x): bool {
        return !$x;
    }

    public function add_one(int $x): int {
        return $x+1;
    }
}
