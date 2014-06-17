<?hh //strict

trait TRepository
{
    private ?PDO $db;
    
    public function __construct( private string $table, ) {}
    
    private function connect(): PDO {
        $db = new PDO('sqlite:../db/database.sqlite3');
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        return $db;
    }
    
    /**
     * Private method to run a query that returns a set of Models
     * 
     * @param string $query The query string itself as a prepared statement
     * @param Map<string, string> $data Array of data to be passed into the query
     * @return Vector<Map<string, string>> The collection of results, can be empty
     */
    private function find(
          string $query,
          QueryData $data = Map{},
        ): Vector<Map<string, string>> {
    
        $db = $this->connect();
        
        $class = 'Post';
        $post = new $this;
        
        $query = $db->prepare($query);
        $query->execute($data);
        $result = $query->fetchAll();
        
        $real_result = Vector{};
        
        foreach($result as $item) {
            $real_result[] = new Map($item);
        }
        
        return $real_result;
    }
}