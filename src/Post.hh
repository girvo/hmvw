<?hh //strict

newtype QueryData = ImmMap<string, mixed>;

class Post
{
    use Model;

    public function __construct(
        public int $id,
        public string $title,
        public string $body,
        public DateTime $posted_at,
        public int $author_id,
    ) {}
}

class PostRepository
{
    use Repository;
    
    public function __construct(): void {}
    
    /**
     * @param int $id The ID of the post to retrieve
     * @return ?Post The hydrated post model
     */
    public function findOne(int $id): ?Post {
        
        $query = 'SELECT * FROM posts WHERE posts.id = :id LIMIT 1';
        $query_data = ImmMap { ":id" => $id };
        
        $result_set = $this->find($query, $query_data);
        
        if ($result_set->isEmpty()) {
            return null;
        }
        
        // get the (only) item
        $result = $result_set->at(0);
        
        // hydrate a new Post model
        return new Post(
            (int) $result['id'],
            $result['title'],
            $result['body'],
            new DateTime($result['created_at']),
            (int) $result['author_id'],
        );
    }
    
    /**
     * Updates a post, that matches the $updated_post->id
     * @param Post $updated_post The updated post object to be persisted.
     * @return bool Success or failure of update
     */
    public function update(Post $updated_post): bool {
        // find the post first
        $original_post_query = "SELECT id FROM posts WHERE posts.id = :id";
        $original_post_data = ImmMap { ":id" => $updated_post->id };
        
        $result_set = $this->find($$original_post_query, $original_post_data);
        
        if ( $result_set->isEmpty() ) {
            return false;
            // throw exception?
        }
        
        $original_post_id = $result_set[0];
        
        return false;
    }
    
    public function doTest(): void {
        $query = "SELECT id FROM posts WHERE posts.id = :id";
        $data = ImmMap { ":id" => 1 };
        $results_set = $this->find($query, $data);
        
        var_dump($results_set[0]);
    }
}

trait Model
{
}

trait Repository
{
    /**
     * Private method to run particular queries for the public method calls
     * 
     * @param string $query The query string itself as a prepared statement
     * @param ImmMap<string, string> $data Array of data to be passed into the query
     * @return Vector<Map<string, string>> The collection of results, can be empty
     */
    private function find(
          string $query,
          QueryData $data,
        ): Vector<Map<string, string>> {
    
        $db = new PDO('sqlite:../db/database.sqlite3');
        
        $query = $db->prepare($query);
        $query->execute($data);
        $result = $query->fetchAll();
        
        $real_result = Vector{};
        
        foreach($result as $item) {
            $real_result[] = new Map($item);
        }
        
        return $real_result;
    }
    
    /*private function runQuery(
        
        ): {
        
        $db = new PDO('sqlite:../db/database.sqlite3');
        
        $query = $db->prepare($query);
        $query->execute($data);
        $result = $query->fetchAll();
    }*/
    
    /**
     * Gets a count of rows that mataches a query
     */
    private function count(string $query, QueryData $data,): int {
        
    }
}