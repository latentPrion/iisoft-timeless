<?php
	if (!isset($_POST))
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Search&error=1" />

<?php
		die();
	};

	$queryCriteria = "";
	$displayAll = 0;

	if ((!isset($_POST['search_timeRemaining']) || $_POST['search_timeRemaining'] == ""))
	{
		// Display all sessions.
		$queryString = "SELECT * FROM session";
		$displayAll = 1;
	}
	else
	{
			$queryCriteria = "(session.timeRemaining >= " . $_POST['search_timeRemaining'] . ")";
	};

	/* Now, if the query Criteria string is not null, we can send in
	 * the query to the ODBC driver.
	 **/
	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Search&error=2" />

<?php
		die("Failed to connect to DB!");
	};

	if (!$displayAll)
	{
		$queryString = 	"SELECT * "
			. "FROM session "
			. "WHERE " . $queryCriteria . " "
			. "ORDER BY session.id DESC";
	};

	$resultHandle = odbc_exec($dbHandle, $queryString);

	// Done querying; create page and move on.
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<head>
	<title>session search results :: Timeless</title>
	<link rel="stylesheet" type="text/css" href="../layout.css" />
	<link rel="icon" type="image/ico" href="../images/favicon.ico" />
</head>

<body>

<div id="topBanner"><img src="../images/jsc-v01_02.gif" alt="" width="100%" /></div>
<div id="addrBar"><img src="../images/jsc-v01_04.png" alt="" /></div>
<div id="topMenu">
	<?php require('../menus/topMenu.php'); ?>
</div>

<div id="contentWrapper">
	<div id="contentTopBrace"><img src="../images/jsc-v01_09.gif" alt="" /></div>
	<table id="contentDivider">
	<tr>
	<td id="menuBar">
		<div id="dbTableMenu"><?php require('../menus/dbTableMenu.php'); ?></div>
	</td>
	<td id="contentBar">
		<div id="contentMain">
			<table>
			<tr>
				<td colspan="3">List of matching session records:</td>
			</tr>
			<tr>
				<td>Id:</td>
				<td>Time remaining:</td>
				<td></td>
			</tr>
<?php
	$numRows = 0;
	while (odbc_fetch_row($resultHandle))
	{
		$numRows++;
?>
			<tr>
				<td><?php echo odbc_result($resultHandle, 'id'); ?></td>
				<td><?php echo odbc_result($resultHandle, 'timeRemaining'); ?> min</td>
				<td><a href="sessionDetails.php?id=<?php echo odbc_result($resultHandle, 'id'); ?>">View details</a></td>
			</tr>
<?php
	};

	odbc_close($dbHandle);
?>
			<tr>
				<td colspan="4"><?php echo $numRows; ?> results in total.</td>
			</tr>
			</table>
		</div>
	</td>
	</tr>
	</table>
	<div id="contentBottomBrace"><img src="../images/jsc-v01_11.gif" alt="" /></div>
</div>

<div id="copyrightFooter">
	<span class="copyright">&copy; 2012 Joanna's Internet Cafe. All rights reserved.</span>
</div>

</body>

</html>

