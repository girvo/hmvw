<?hh //partial

class Router
{
    private Vector $urls;
    
    public function __construct() {
        $urls = Vector {};
    }
    
    // actions to add a route
    public function get(string $uri, string $function): void {}
    public function post(string $uri, string $function): void {}
    public function put(string $uri, string $function): void {}
    public function delete(string $uri, string $function): void {}
    
    private function matchAction(): void {}
    private function getRequest(): void {}
    
    // final method to kick the app off!
    public function run(): void {
        $request = $this->getRequest();
        $this->matchAction($request);
    }
}