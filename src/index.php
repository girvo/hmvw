<?hh //partial

require "Post.hh";
require "User.hh";

function pre($string) {
    print '<pre>';
    print $string;
    print '</pre>';
}

$postRepo = new PostRepository();

$postRepo->doTest();