<html>
<head>
<title>Welcome to GUARDIAN!</title>
<style>
body{
background-color:black;
}
#title{
text-align:center;
margin-top:25%;
font-weight:bold;
font-size:40px;
color:white;
}
#macarea{
margin-top:10%;
color:white;
font-size:25px;
background-color:black;
border-style:solid;
border-width:3px;
border-color:white;
width:85%;
text-align:center;
padding:10px;
}
#namearea{
margin-top:4%;
color:white;
font-size:25px;
background-color:black;
border-style:solid;
border-width:3px;
border-color:white;
width:85%;
text-align:center;
padding:10px;
}
#join{
text-align:center;
}
#submitarea
{
margin-top:8%;
color:red;
background-color:black;
font-size:25px;
text-align:center;
border-width:5px;
width:50%;
border-style:solid;
border-color:red;
padding:10px;
font-weight:bold;
}
</style>
<script>
var check=true;
var cl=document.getElementById(namearea);
function clearMessage(frm){
if(check)
{
frm.name.value="";
check=false;
}
}
window.history.forward();
function noback()
{
window.history.forward();
}
</script>
</head>
<body onload="noback();"  onpageshow="if(event.persisted){noback();}">
<?php
$mac=$_POST["mac"];
?>
<p id="title">GUARDIAN</p>
<div id="img1" style="text-align:center;margin-top:-20px">
<img src="1.png" />
</div>
<div id="join">
<form name="form" method="post" action="join.php">
<input type="text" name="mac" id="macarea" value="<?=$mac?>" readonly  />
<input type="text" name="name" id="namearea" value="Put Your Name" onFocus="clearMessage(this.form);">
<input type="submit" id="submitarea" value="REGISTER" />
</form>
</div>
</body>
</html>
