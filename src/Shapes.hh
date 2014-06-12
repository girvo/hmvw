<?hh

type PostShape = shape(
    'id' => int, 
    'title' => string,
    'postedAt' => DateTime,
    'body' => string,
    'author' => User
);

type UserShape = shape(
    'id' => int,
    'email_address' => string,
    'real_name' => string,
    'password' => string
);