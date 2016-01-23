<html>
	<body>
		Add new actor in a movie: <br/>

<?php 

$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);

echo "<form action=\"./addMovieActor.php\" method=\"GET\">";

//selector Movie
$query = "SELECT id, title,year FROM Movie ORDER BY title;";
$rs = mysql_query($query, $db_connection);
$row = mysql_fetch_row($rs);

echo'Movie :<select name="mid">';
while($row = mysql_fetch_row($rs)){
echo"<option value=".$row[0].">".$row[1]." (".$row[2].")"."</option>";
}
echo'</select><br>';

//select Actor
$query = "SELECT id, first,last,dob FROM Actor ORDER BY first, last;";
$rs = mysql_query($query, $db_connection);
$row = mysql_fetch_row($rs);
echo'Actor :<select name="aid">';
while($row = mysql_fetch_row($rs)){
echo"<option value=".$row[0].">".$row[1]." ".$row[2]." (".$row[3].")"."</option>";
}
echo'</select><br>';

echo "Role :<input type=\"text\" name=\"role\" maxlength=\"50\">";
echo '<br>';

echo "<INPUT TYPE=\"submit\" VALUE=\"Add it!!\">";

mysql_close($db_connection);
echo "</form>";


if($mid = $_GET["mid"]){
	$db_connection=mysql_connect('localhost',"cs143","");
	mysql_select_db("CS143", $db_connection);
	$aid = $_GET["aid"];
	$role = $_GET["role"];
	$query = "INSERT INTO MovieActor VALUES($mid, $aid, \"$role\");";
	mysql_query($query, $db_connection);
	echo "Add Success!";
	mysql_close($db_connection);
}


?>

</body>

</html>