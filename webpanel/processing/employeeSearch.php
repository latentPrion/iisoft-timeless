<?php
	if (!isset($_POST))
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Search&error=1" />

<?php
		die();
	};

	$queryCriteria = "";
	$displayAll = 0;

	if ((!isset($_POST['search_fullName']) || $_POST['search_fullName'] == "")
		&& (!isset($_POST['search_email']) || $_POST['search_email'] == "")
		&& (!isset($_POST['search_id']) || $_POST['search_id'] == ""))
	{
		// Display all customers.
		$queryString = "SELECT * FROM employee";
		$displayAll = 1;
	}
	else
	{
		if (isset($_POST['search_fullName']) && $_POST['search_fullName'] != "") {
			$queryCriteria = "(employee.fullName = '" . $_POST['search_fullName'] . "'";
		};

		if (isset($_POST['search_email']) && $_POST['search_email'] != "")
		{
			if ($queryCriteria == "") {
				$queryCriteria = "(employee.email = '" . $_POST['search_email'] . "'";
			}
			else {
				$queryCriteria = $queryCriteria . ") AND (employee.email = '" . $_POST['search_email'] . "'";
			};
		};

		if (isset($_POST['search_id']) && $_POST['search_id'] != "")
		{
			if ($queryCriteria == "") {
				$queryCriteria = "(employee.id = " . $_POST['search_id'];
			}
			else {
				$queryCriteria = $queryCriteria . ") AND (employee.id = " . $_POST['search_id'];
			};
		};

		$queryCriteria = $queryCriteria . ")";
	};

	/* Now, if the query Criteria string is not null, we can send in
	 * the query to the ODBC driver.
	 **/
	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Search&error=2" />

<?php
		die("Failed to connect to DB!");
	};

	if (!$displayAll)
	{
		$queryString = 	"SELECT employee.fullName, employee.id, employee.jobDescription "
			. "FROM employee "
			. "WHERE " . $queryCriteria . " "
			. "ORDER BY employee.id ASC";
	};

	$resultHandle = @odbc_exec($dbHandle, $queryString);

	// Done querying; create page and move on.
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<head>
	<title>Employee search results :: Timeless</title>
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
				<td colspan="4">List of matching employee records:</td>
			</tr>
			<tr>
				<td>Id:</td>
				<td>Name:</td>
				<td>Job description:</td>
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
				<td><?php echo odbc_result($resultHandle, 'fullName'); ?></td>
				<td><?php echo odbc_result($resultHandle, 'jobDescription'); ?></td>
				<td><a href="employeeDetails.php?id=<?php echo odbc_result($resultHandle, 'id'); ?>">View details</a></td>
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

