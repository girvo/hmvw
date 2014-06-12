<?hh //strict
class User
{
    public function __construct(
        public int $id,
        public string $email,
        public string $password,
        public string $real_name
    ): void {}
}

class UserRepository
{    
    /*public static function findOne(int $id): ?User { }
    public static function findOneByEmail(string $email): ?User { }
    public static function updateById(int $id, User $user): bool { }*/
}