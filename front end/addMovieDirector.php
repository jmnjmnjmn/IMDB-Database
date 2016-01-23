<html>
	<body>
		Add new director in a movie: <br/>

<?php 
$mid=$_GET['mid'];
$did=$_GET['did'];

$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);

echo "<form action=\"./addMovieDirector.php\" method=\"GET\">";

//selector Movie
$query = "SELECT id, title,year FROM Movie order by title;";
$rs = mysql_query($query, $db_connection);
$row = mysql_fetch_row($rs);

echo'Movie: <select name="mid">';
while($row = mysql_fetch_row($rs)){
	echo"<option value=".$row[0].">".$row[1]." (".$row[2].")"."</option>";
}
echo'</select><br>';

//select Actor
$query = "SELECT id, first,last,dob FROM Director order by first, last;";
$rs = mysql_query($query, $db_connection);
$row = mysql_fetch_row($rs);
echo'Director: <select name="did">';
while($row = mysql_fetch_row($rs)){
echo"<option value=".$row[0].">".$row[1]." ".$row[2]." (".$row[3].")"."</option>";
}
echo'</select><br>';
echo "<INPUT TYPE=\"submit\" VALUE=\"Add it!!\">";
echo "</form>";

//insert MovieDirector
if($_GET['mid']){
$insertMovieDirector = "insert into MovieDirector values($mid,$did)";
$rs = mysql_query($insertMovieDirector, $db_connection);
echo "<br><br><hr/>
Add Success!<br>";
}
mysql_close($db_connection);



?>

</body>

</html>