<?php
	if (!isset($_POST))
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=Search&error=1" />

<?php
		die();
	};

	$queryCriteria = "";
	$displayAll = 0;

	if ((!isset($_POST['search_description']) || $_POST['search_description'] == "")
		&& (!isset($_POST['search_id']) || $_POST['search_id'] == ""))
	{
		// Display all pcs.
		$queryString = "SELECT * FROM pc";
		$displayAll = 1;
	}
	else
	{
		if (isset($_POST['search_description']) && $_POST['search_description'] != "") {
			$queryCriteria = "(pc.description = '" . $_POST['search_description'] . "'";
		};

		if (isset($_POST['search_id']) && $_POST['search_id'] != "")
		{
			if ($queryCriteria == "") {
				$queryCriteria = "(pc.id = " . $_POST['search_id'];
			}
			else {
				$queryCriteria = $queryCriteria . ") AND (pc.id = " . $_POST['search_id'];
			};
		};

		$queryCriteria = $queryCriteria . ")";
	};

	/* Now, if the query Criteria string is not null, we can send in
	 * the query to the ODBC driver.
	 **/
	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=Search&error=2" />

<?php
		die("Failed to connect to DB!");
	};

	if (!$displayAll)
	{
		$queryString = 	"SELECT pc.description, pc.id "
			. "FROM pc "
			. "WHERE " . $queryCriteria . " "
			. "ORDER BY pc.id ASC";
	};

	$resultHandle = odbc_exec($dbHandle, $queryString);

	// Done querying; create page and move on.
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<head>
	<title>PC search results :: Timeless</title>
	<link rel="stylesheet" type="text/css" href="../layout.css" />
	<link rel="icon" type="image/ico" href="../images/favicon.ico" />
</head>

<body>

<div id="topBanner"><img src="../images/jsc-v01_02.gif" alt="" /></div>
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
				<td colspan="3">List of matching PC records:</td>
			</tr>
			<tr>
				<td>Id:</td>
				<td>Description:</td>
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
				<td><?php echo odbc_result($resultHandle, 'description'); ?></td>
				<td><a href="pcDetails.php?id=<?php echo odbc_result($resultHandle, 'id'); ?>">View details</a></td>
			</tr>
<?php
	};

	odbc_close($dbHandle);
?>
			<tr>
				<td colspan="3"><?php echo $numRows; ?> results in total.</td>
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

