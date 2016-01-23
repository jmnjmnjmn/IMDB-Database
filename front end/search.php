<!DOCTYPE html>
<html>
<title>Search Actor / Movie</title>

<body>
Search for actors/movies:<br>
<form action="<?php echo $_SERVER['PHP_SELF'];?>"  method="GET" >
	Search:<input type="text" name="keyword"></input>
	<INPUT TYPE="submit" VALUE="search">
</form>

<?php 

if($area = $_GET['keyword']){

$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);

$arr=split(' ',$area);

//search from Actor
if(count($arr)==1){
$searchActor="select id,first,last,dob from Actor where first like \"%$area%\" or last like \"%$area%\" order by first, last";}
else if(count($arr)==2){
$searchActor="select id,first,last,dob from Actor where (first like \"$arr[0]\" and last like \"$arr[1]%\") or
 (first like \"$arr[1]\" and last like \"$arr[0]%\") order by first, last";}
else{$searchActor="select * from Actor where 0>1";}

$rs = mysql_query($searchActor, $db_connection);

//search from Movie
$searchMovie="select id,title,year from Movie where title like \"%$area%\" order by title";
$rs1 = mysql_query($searchMovie, $db_connection);

echo "<h3>You are searching [$area] results...<br><br>";
echo "Searching match records in Actor database ...</h3>";
while($row = mysql_fetch_row($rs)) {
   	echo "<tr  align=\"center\"> Actor:";
	echo "<a href=\"./showActorInfo.php?aid=$row[0]\">";
	echo $row[1]." ".$row[2]."(".$row[3].")";   
	echo "</a>";
    echo "</tr><br>\n";
}


echo "<h3>Searching match records in Movie database ...</h3>";
while($row = mysql_fetch_row($rs1)) {
    
	echo "<tr  align=\"center\"> Movie:";
	echo "<a href=\"./showMovieInfo.php?mid=$row[0]\">";
	echo $row[1]."(".$row[2].")";  
	echo "</a>";
    echo "</tr><br>\n";
}

mysql_close($db_connection);
}
?>

</body>
</html>