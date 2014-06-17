<?hh //strict

trait Repository
{
    private ?PDO $db;
    private string $table;
    
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
    
    /**
     * Private impl to delete a single item, based on ID. 
     * Expects there to be an ':id' key on the QueryData Map, so you can pass a 
     * model->asQueryData() directly
     */
    private function delete(
          QueryData $item,
        ): bool {
        
        if (!$item->containsKey(':id')) {
            throw new Exception('No ID in query data to delete?');
        }
        
        $sql = 'DELETE FROM '.$this->table.' WHERE id = :id';
        $db = $this->connect();
        $query = $db->prepare($sql);
        $query->bindValue(':id', $item[':id']);
        $query->execute();
        
        if (!$query->rowCount()) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Private method to insert a single item, loops over a $model->asQueryData
     * map and creates the correct PDO query to insert it, returns the ID of 
     * the inserted row. Will ignore ':id'.
     */
    private function insert(
          QueryData $item,
        ): int {
        // get rid of the ':id' or 'id' column        
        if ($item->containsKey(':id')) {
            $item = $item->remove(':id');
        }
        
        if ($item->containsKey('id')) {
            $item = $item->remove('id');
        }
        
        // build the column/value strings
        $col_string = '';
        $val_string = '';
        
        $keys = $item->keys();
        
        foreach ($keys as $col) {
            // setup the column names
            $col = (string) $col;
            if ($col[0] === ':') {
                $col_string .= (string) substr($col, 1); // remove the ':'
            } else {
                $col_string .= $col;
            }
            $col_string .= ', ';
            
            // now setup the value replacement tokens
            $val_string .= $col . ', ';
        }
        
        $col_string = (string) substr($col_string, 0, -2); // trim the final ', '
        $val_string = (string) substr($val_string, 0, -2); // same

        $sql = 'INSERT INTO '.$this->table.' ('.$col_string.') VALUES ('.$val_string.')';
        
        $db = $this->connect();
        $query = $db->prepare($sql);
        $query->execute($item);
        
        $id = (int) $db->lastInsertId();
        
        if ($id > 0) {
            return $id;
        } else {
            throw new Exception('Error inserting item');
        }
    }
}