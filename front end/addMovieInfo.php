<html>
<body>
				Add new movie: <br/>
		<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="GET">			
			Title : <input type="text" name="title" maxlength="20"><br/>
			Compnay: <input type="text" name="company" maxlength="50"><br/>
			Year : <input type="text" name="year" maxlength="4"><br/>	<!-- Todo: validation-->	
			MPAA Rating : <select name="mpaarating">
					<option value="G">G</option>
<option value="NC-17">NC-17</option>
<option value="PG">PG</option>
<option value="PG-13">PG-13</option>
<option value="R">R</option>
<option value="surrendere">surrendere</option>
					</select>
			<br/>
			Genre : 
					<input type="checkbox" name="genre_Action" value="Action">Action</input>
<input type="checkbox" name="genre_Adult" value="Adult">Adult</input>
<input type="checkbox" name="genre_Adventure" value="Adventure">Adventure</input>
<input type="checkbox" name="genre_Animation" value="Animation">Animation</input>
<input type="checkbox" name="genre_Comedy" value="Comedy">Comedy</input>
<input type="checkbox" name="genre_Crime" value="Crime">Crime</input>
<input type="checkbox" name="genre_Documentary" value="Documentary">Documentary</input>
<input type="checkbox" name="genre_Drama" value="Drama">Drama</input>
<input type="checkbox" name="genre_Family" value="Family">Family</input>
<input type="checkbox" name="genre_Fantasy" value="Fantasy">Fantasy</input>
<input type="checkbox" name="genre_Horror" value="Horror">Horror</input>
<input type="checkbox" name="genre_Musical" value="Musical">Musical</input>
<input type="checkbox" name="genre_Mystery" value="Mystery">Mystery</input>
<input type="checkbox" name="genre_Romance" value="Romance">Romance</input>
<input type="checkbox" name="genre_Sci-Fi" value="Sci-Fi">Sci-Fi</input>
<input type="checkbox" name="genre_Short" value="Short">Short</input>
<input type="checkbox" name="genre_Thriller" value="Thriller">Thriller</input>
<input type="checkbox" name="genre_War" value="War">War</input>
<input type="checkbox" name="genre_Western" value="Western">Western</input>		
			<br/>
			
			<input type="submit" value="Add it!!"/>
					</form>

<?php 

if($title = $_GET["title"]){

	$db_connection=mysql_connect('localhost',"cs143","");
	mysql_select_db("CS143", $db_connection);

	$company = $_GET["company"];
	$year = $_GET["year"];
	$mpaarating = $_GET["mpaarating"];

	$query1 = "SELECT COUNT(*) FROM MaxMovieID;";
	$rs = mysql_query($query1, $db_connection);
	$row = mysql_fetch_row($rs);
	if ($row[0] == 0){
		$query2 = "INSERT INTO MaxMovieID (SELECT MAX(id) FROM Movie);";
		mysql_query($query2, $db_connection);
	}

	$query3 = "UPDATE MaxMovieID SET id = (SELECT MAX(id) FROM Movie)+1;";
	mysql_query($query3, $db_connection);

	$query4 = "SELECT id FROM MaxMovieID;";
	$rs1 = mysql_query($query4, $db_connection);
	$row1 = mysql_fetch_row($rs1);
	$query = "INSERT INTO Movie VALUES($row1[0], \"$title\", $year ,\"$mpaarating\",\"$company\");";
	mysql_query($query, $db_connection);


	if($genre_Action = $_GET["genre_Action"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Action\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Adult = $_GET["genre_Adult"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Adult\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Adventure = $_GET["genre_Adventure"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Adventure\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Animation = $_GET["genre_Animation"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Animation\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Comedy = $_GET["genre_Comedy"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Comedy\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Crime = $_GET["genre_Crime"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Crime\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Documentary = $_GET["genre_Documentary"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Documentary\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Drama = $_GET["genre_Drama"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Drama\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Family = $_GET["genre_Family"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Family\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Fantasy = $_GET["genre_Fantasy"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Fantasy\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Horror = $_GET["genre_Horror"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Horror\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Musical = $_GET["genre_Musical"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Musical\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Mystery = $_GET["genre_Mystery"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Mystery\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Romance = $_GET["genre_Romance"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Romance\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Sci_Fi = $_GET["genre_Sci-Fi"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Sci_Fi\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Short = $_GET["genre_Short"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Short\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Thriller = $_GET["genre_Thriller"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Thriller\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_War = $_GET["genre_War"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_War\");";
		mysql_query($query5, $db_connection);
	}
	if($genre_Western = $_GET["genre_Western"]){
		$query5 = "INSERT INTO MovieGenre VALUES($row1[0], \"$genre_Western\");";
		mysql_query($query5, $db_connection);
	}
echo "Add Success!";

mysql_close($db_connection);

}

?>

		<hr/>				
	</body>
</html>