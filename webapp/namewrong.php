<html>
<head>
<title> Put Your Name!!! </title>
<style>
p{
font-size:30px;
text-align:center;
color:white;
font-weight:bold;
margin-bottom:2%;
}
</style>

</head>
<body style="background-color:black">
<?php
$mac=$_POST["mac"];
echo $mac;
?>
<div id="pp" style="margin-top:50%; text-align:center">
<p>Modify</p>
<p>YOUR NAME</p>
<form name="form1" method='post' action='register.php'>
<input type='hidden' name="mac" value="<?=$mac?>" />
</form>
<script>
setTimeout("document.form1.submit();",1500);
</script>

</div>
</body>
</html>
