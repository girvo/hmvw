<?hh //strict

newtype QueryData = Map<string, mixed>;

class Post
{
    use Model;

    public function __construct(
        public int $id,
        public string $title,
        public string $body,
        public DateTime $created_at,
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
    public function findOne(int $id): Post {
        
        $query = 'SELECT * FROM posts WHERE posts.id = :id LIMIT 1';
        $query_data = Map { ":id" => $id };
        
        $result_set = $this->find($query, $query_data);
        
        if ($result_set->isEmpty()) {
            //return null;
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

        $update_query = "UPDATE posts SET 
                        title = :title, 
                        body = :body,
                        created_at = :created_at,
                        author_id = :author_id
                        WHERE `posts`.id = :id";
        $this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
        $query = $this->db->prepare($update_query);
        $data = $updated_post->asQueryData();

        $query->execute($data);

        var_dump($query->rowCount());
        
        return false;
    }
    
    public function doTest(): void {
        $orig = $this->findOne(1);
        
        var_dump($orig);
        
        print "<br/><br/>";
        
        $new = $orig;
        
        $new->title = "This is an update test";
        
        $this->update($new);
        
        $updated = $this->findOne(1);
        
        var_dump($updated);
    }
    
}

trait Model
{
    public function asQueryData(): Map<string, mixed> {
        $result = Map {};
        $props = get_object_vars($this);
        
        foreach ($props as $property => $value) {
            // nasty :/
            if ($value instanceof DateTime) {
                $result[':'.$property] = $value->format('Y-m-d h:m:s');
            } else {
                $result[':'.$property] = $value;
            }
        }

        return $result;
    }
}

trait Repository
{
    /**
     * Private method to run particular queries for the public method calls
     * 
     * @param string $query The query string itself as a prepared statement
     * @param Map<string, string> $data Array of data to be passed into the query
     * @return Vector<Map<string, string>> The collection of results, can be empty
     */
    private PDO $db;
     
    public function __construct(): void {
        $this->db = new PDO('sqlite:../db/database.sqlite3');
        $this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }
     
    private function find(
          string $query,
          QueryData $data,
        ): Vector<Map<string, string>> {
    
        $this->db = new PDO('sqlite:../db/database.sqlite3');
        
        $query = $this->db->prepare($query);
        $query->execute($data);
        $result = $query->fetchAll();
        
        $real_result = Vector{};
        
        foreach($result as $item) {
            $real_result[] = new Map($item);
        }
        
        return $real_result;
    }
}