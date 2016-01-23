<!DOCTYPE html>
<html>
<title>Actor Information</title>

<?php 

$mid = $_GET['mid'];
$yourname = $_GET['yourname'];
$rating = $_GET['rating'];
$comment = $_GET['comment'];

$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);

$showActorInfo="select title,year from Movie where id=$mid";
$rs = mysql_query($showActorInfo, $db_connection);
$row = mysql_fetch_row($rs);


echo "<form action=\"./addComment.php\" method=\"GET\">";
echo "<h3>Add new comment:<br>";
//get movie title year 
echo "Movie:";
echo'<select name="mid">';
echo"<option value=".$mid.">".$row[0]."(".$row[1].")"."</option>";
echo'</select><br>';

//get user name
echo "Your Name:	<input type=\"text\" name=\"yourname\" value=\"Mr. Anonymous\" maxlength=\"20\"><br/>";

//get rating
echo "	Rating:	<select name=\"rating\">
				<option value=5> 5 - Excellent </option>
				<option value=4> 4 - Good </option>
				<option value=3> 3 - It's ok~ </option>
				<option value=2> 2 - Not worth </option>
				<option value=1> 1 - I hate it </option>
			</select>
		<br/>";

//get comment
echo "Comments: <br/>";
echo "<textarea name=\"comment\" cols=\"80\" rows=\"10\"></textarea>
			<br/>
			<input type=\"submit\" value=\"Rate it!!\"/>";

//insert review
if($_GET['rating']){
$time = mysql_query("select now()", $db_connection);
$t = mysql_fetch_row($time);
$insertReview = "insert into Review values(\"$yourname\",\"$t[0]\",$mid,$rating,\"$comment\")";
$rs = mysql_query($insertReview, $db_connection);

echo "<br><br><hr/>
Thanks your comment!! We appreciate it!!<br>
<a href = './showMovieInfo.php?mid=$mid'>See Movie Info (including others' reviews)</a>"	;	
}

echo "</form>";
mysql_close($db_connection);


?>

</html>
