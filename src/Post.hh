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
    
    public function __construct() {}
    
    /**
     * @param int $id The ID of the post to retrieve
     * @return ?Post The hydrated post model
     */
    public function findOne(int $id): Post {
        
        $query = 'SELECT * FROM posts WHERE posts.id = :id LIMIT 1';
        $query_data = Map { ':id' => $id };
        
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
    
    public function findMany(
        string $column, 
        string $operator,
        string $comparison,
      ): Vector<Post> {
        
        $base_query = 'SELECT * FROM posts WHERE :column :operator :comparison';
        
        $query_data = Map { 
            ':column' => $column,
            ':operator' => $operator,
            ':comparison' => $comparison,
        };
        
        $db = $this->connect();
        $query = $db->prepare($base_query);
        
        $results = $query->execute($query_data);
        var_dump($results);
        die;
    }
    
    /**
     * Updates a post, that matches the $updated_post->id
     * @param Post $updated_post The updated post object to be persisted.
     * @return bool Success or failure of update
     */
    public function update(Post $updated_post): bool {

        $update_query = 'UPDATE posts SET 
                        title = :title, 
                        body = :body,
                        created_at = :created_at,
                        author_id = :author_id
                        WHERE `posts`.id = :id';
        $db = $this->connect();
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
        $query = $db->prepare($update_query);
        $data = $updated_post->asQueryData();

        $query->execute($data);

        if ($query->rowCount() > 0) {
            return true;
        }
        
        return false;
    }
    
    public function create(Post $new_post): Post {
        $insert_query = 'INSERT INTO posts (title, body, created_at, author_id)
                         VALUES (:title, :body, :created_at, :author_id)';
                         
        $insert_data = $new_post->asQueryData(Set{ 'title', 'body', 'created_at', 'author_id', });
        
        //$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $db = $this->connect();
        $query = $db->prepare($insert_query);
        $query->execute($insert_data);
        
        $new_post->id = $db->lastInsertId();
        
        return $new_post;
    }
    
    public function delete(Post $post): bool {
        $delete_query = 'DELETE FROM posts WHERE id = :id';
        
        $delete_data = $post->asQueryData(Set{ 'id' });
        
        $db = $this->connect();
        $query = $db->prepare($delete_query);
        $query->execute($delete_data);
        
        if ($query->rowCount() > 0) {
            return true;
        }
        
        return false;
    }
    
    public function doTest(): void {
    
        //$new_post = new Post(-1, 'A New Post', 'This is some body text', new DateTime('NOW'), 1);
        //$posts = $this->findMany('`id`', '>', '1');
        
        $db = $this->connect();
        $query = $db->prepare('SELECT * FROM posts WHERE :fragment');
        
        $query->execute(Map {':fragment' => 'posts.id = 1'});
        $results = $query->fetchAll();
        var_dump($results);
        die;
    }
    
}

trait Model
{
    public function asQueryData(?Set<string> $wanted_props = null): Map<string, mixed> {
        $result = Map {};
        $props = get_object_vars($this);
        
        if ($wanted_props) {
        
            foreach ($props as $property => $value) {
                $property_str = (string) $property;
            
                if ($wanted_props->contains($property_str)) {
                    if ($value instanceof DateTime) {
                        $result[':'.$property_str] = (string) $value->format('Y-m-d h:m:s');
                    } else {
                        $result[':'.$property_str] = $value;
                    }
                }
            }
            
        } else {     
        
            foreach ($props as $property => $value) {
                // nasty :/
                $property_str = (string) $property;

                // really nasty :(
                if ($value instanceof DateTime) {
                    $result[':'.$property_str] = (string) $value->format('Y-m-d h:m:s');
                } else {
                    $result[':'.$property_str] = $value;
                }
            }
            
        }
        
        return $result;
    }
}

trait Repository
{
    private PDO $db;
    
    public function __construct(): void {}
    
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