<!DOCTYPE html>
<html>
<title>Actor Information</title>

<?php 
$aid = $_GET['aid'];


$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);

$showActorInfo="select first,last,dob,dod,sex from Actor where id=$aid";
$rs = mysql_query($showActorInfo, $db_connection);
$row = mysql_fetch_row($rs);

echo "<h3>--Show Actor Info--</h3>";
echo "Name: ".$row[0]." ".$row[1]."<br>";
echo "Sex: ".$row[4]."<br>";
echo "Date Of Birth: ".$row[2]."<br>";
if ($row[3]==null)
	echo "Data Of Death:--Still Alive--<br>";
else
	echo "Data Of Death:".$row[3]."<br>";
	echo "<br><br>";


$showActorInfo="select role,title,mid from MovieActor,Movie where aid=$aid and mid=Movie.id";
$rs = mysql_query($showActorInfo, $db_connection);
echo "<h3>--Act in--</h3>";
while($row = mysql_fetch_row($rs)) {
    echo "Act \"".$row[0]."\" in ";
	echo "<a href=\"./showMovieInfo.php?mid=$row[2]\">";
    echo $row[1]."</a><br>";
}


mysql_close($db_connection);


?>
<br>
<br>
<hr/>
Search for actors/movies:<br>
<form action="./search.php"  method="GET" >
	Search:<input type="text" name="keyword"></input>
	<INPUT TYPE="submit" VALUE="search">
</form>

</html>