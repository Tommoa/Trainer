# Trainer
An extendable trainer for games

## File format
`create` `type` `name` `offsets` - Creates a variable of type `type` with name `name` with memory offsets `offsets`.  
`set` `name` `value` - Sets variable `name` to value `value`.  
`get` `name` - Gets the value of variable `name`.  
`export` `filetype` `filename` - exports a file `filename` with either the file type `binary` or `file`.  
`import` `filetype` `filename` - imports a file `filename` into the current session. File type is either `binary` or `file`.  
