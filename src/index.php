<?hh //partial

require "Repository.hh";
require "Post.hh";
/*require "User.hh";
require "Router.hh";

function pre($string) {
    print '<pre>';
    print $string;
    print '</pre>';
}

$app = new Router();

$app->get('/', 'SomeController:someMethod');
$app->post('/test/:id', 'SomeThingToTest');

$app->run();*/

$PostsRepo = new PostRepository();

$PostsRepo->doTest();