<!DOCTYPE html>
<html>
<title>Actor Information</title>

<?php 
$mid = $_GET['mid'];


$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);

//last,first,dob,genre
//Director D,MovieGenre MG,MovieDirector MD where M.id=$mid and M.id=MG.mid and M.id=MD.mid and D.id=MD.did";
$showMovieInfo="select title,year,company,rating from Movie where id=$mid";
$rs = mysql_query($showMovieInfo, $db_connection);
$row = mysql_fetch_row($rs);

echo "<h3>--Show Movie Info--</h3>";
echo "Title: ".$row[0]."(".$row[1].")"."<br>";
echo "Producer: ".$row[2]."<br>";
echo "MPAA Rating: ".$row[3]."<br>";



//Movie Info
$showMovieInfo="select first,last,dob from Director,MovieDirector where mid=$mid and did=Director.id";
$rs = mysql_query($showMovieInfo, $db_connection);
echo "Director: ";
$count=0;
while($row = mysql_fetch_row($rs)) {
	if ($count==0)
	echo $row[0]." ".$row[1]."(".$row[2].")";    
	else
	echo ",".$row[0]." ".$row[1]."(".$row[2].")";    
    $count++;
}
echo "<br>";
$showMovieInfo="select genre from MovieGenre where mid=$mid";
$rs = mysql_query($showMovieInfo, $db_connection);
echo "Genre: ";
$count=0;
while($row = mysql_fetch_row($rs)) {
	if ($count==0)
	echo $row[0];    
	else
	echo ",".$row[0];  
 	$count++;
}
echo "<br><br>";
//Actor in this movie
$showActorInfo="select first,last,role,id from MovieActor,Actor where mid=$mid and aid=id";
$rs = mysql_query($showActorInfo, $db_connection);
echo "<h3>--Actor in this movie--</h3>";
while($row = mysql_fetch_row($rs)) {
    echo "<a href=\"./showActorInfo.php?aid=$row[3]\">";
    echo $row[0]." ".$row[1];
	echo "</a>";
    echo " act as"."\"$row[2]\"";
	echo "<br>";
}

echo "<br><br>";


//User Review
echo "<h3>--User Review--</h3>";
$showActorInfo="select avg(rating),count(*) from Review where mid=$mid";
$rs = mysql_query($showActorInfo, $db_connection);
while($row = mysql_fetch_row($rs)) {
	echo " Average Score: ".$row[0]."/5 (5.0 is best) by ".$row[1]." reviews(s).";
    echo "<a href=\"./addComment.php?mid=$mid\">";
    echo " Add your review now!!<br>";
	echo "</a><br>";
}


echo "All Comments in Details:<br>";
$showActorInfo="select time,name,rating,comment from Review where mid=$mid";
$rs = mysql_query($showActorInfo, $db_connection);
while($row = mysql_fetch_row($rs)) {
	echo "In ".$row[0]." ".$row[1]." said: I rate this movie score ".$row[2]." point(s), here is my comment:<br>";
    echo $row[3]."<br><br>";
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

