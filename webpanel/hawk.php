<?php
	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

Failed to connect to the database!

<?php
		die();
	};

	$queryString = "SELECT * from pc WHERE currentSessionId <> 0";
	$resultHandle = odbc_exec($dbHandle, $queryString);
?>

<meta http-equiv="refresh" content="5;hawk.php" />
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<head>
	<title>Main page :: Timeless</title>
	<link rel="stylesheet" type="text/css" href="layout.css" />
	<link rel="icon" type="image/ico" href="images/favicon.ico" />
</head>

<body>

<div id="topBanner"><img src="images/jsc-v01_02.gif" alt=""  width="100%" /></div>
<div id="addrBar"><img src="images/jsc-v01_04.png" alt="" /></div>
<div id="topMenu">
	<?php require('menus/topMenu.php'); ?>
</div>

<div id="contentWrapper">
	<div id="contentTopBrace"><img src="images/jsc-v01_09.gif" alt="" /></div>
	<table id="contentDivider">
	<tr>
	<td id="menuBar">
		<div id="dbTableMenu"><?php require('menus/dbTableMenu.php'); ?></div>
	</td>
	<td id="contentBar">
		<div id="contentMain">
		<table width="100%">
			<tr>
				<td>All active sessions:</td>
			</tr>
<?php
	while (odbc_fetch_row($resultHandle))
	{
?>
			<tr><td colspan="2"><hr width="100%" /></td></tr>
			<tr>
				<td>PC #<?php echo odbc_result($resultHandle, "id"); ?></td>
				<td>Session #<?php echo odbc_result($resultHandle, "currentSessionId"); ?>.</td>
			</tr>
			<tr>
<?php
		$queryString2 = "SELECT * FROM session WHERE id = " . odbc_result($resultHandle, "currentSessionId");
		$result2 = odbc_exec($dbHandle, $queryString2);
		odbc_fetch_row($result2);
		if (odbc_result($result2, "type") == 1)
		{
?>
				<td style="background-color:
				<?php
					if (odbc_result($result2, 'timeRemaining') <= 0) {
						echo '#cc7777';
					} else {
						echo '#77cc77';
					};
				?>;">Time remaining: <?php echo odbc_result($result2, "timeRemaining"); ?> min</td>
				<td>(Temporary session)</td>
<?php
		}
		else
		{
			$queryString3 = "SELECT * FROM customer WHERE id = " . odbc_result($result2, "customerId");
			$result3 = odbc_exec($dbHandle, $queryString3);
			odbc_fetch_row($result3);
?>
				<td style="background:
				<?php
					if (odbc_result($result3, 'timeRemaining') <= 0) {
						echo '#cc7777';
					} else {
						echo '#77cc77';
					};
				?>">Time remaining: <?php echo odbc_result($result3, "timeRemaining"); ?> min</td>
				<td>(Normal Session)</td>
<?php
		};
?>
			</tr>
			<tr>
				<td colspan="2"><a href="fclose.php?id=<?php echo odbc_result($resultHandle, "currentSessionId"); ?>">Force close</a>.</td>
			</tr>
<?php
	};
	odbc_close($dbHandle);
?>
		</table>

		</div>
	</td>
	</tr>
	</table>
	<div id="contentBottomBrace"><img src="images/jsc-v01_11.gif" alt="" /></div>
</div>

<div id="copyrightFooter">
	<span class="copyright">&copy; 2012 <a href="#" onclick="javascript: alert('PC #5, Session \'SES008\' has expired!');">Joanna's</a> Internet Cafe. All rights reserved.</span>
</div>

</body>

</html>

