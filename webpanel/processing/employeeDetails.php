<?php
	if (!isset($_REQUEST['id']))
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=List&mode=details" />

<?php
		die();
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
		die ("Could not connect to DB!");
	};

	// Take the required customer ID and project all columns from DB.
	$resultHandle = odbc_exec(
		$dbHandle,
		"SELECT * "
		. "FROM employee "
/*			. "INNER JOIN employee "
			. "ON customer.employeeId = employee.id " */
		. "WHERE employee.id = " . $_REQUEST['id']);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<head>
	<title>Employee #<?php echo $_REQUEST['id'] ?> :: Details :: Timeless</title>
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
<?php
	if (odbc_fetch_row($resultHandle))
	{
?>
			<tr>
				<td>Id:</td>
				<td><?php echo odbc_result($resultHandle, "id"); ?></td>
			</tr>
			<tr>
				<td>Name:</td>
				<td><?php echo odbc_result($resultHandle, "fullName"); ?></td>
			</tr>
			<tr>
				<td>Email:</td>
				<td><?php echo odbc_result($resultHandle, "email"); ?></td>
			</tr>
			<tr>
				<td>Address:</td>
				<td><?php echo odbc_result($resultHandle, "address"); ?></td>
			</tr>
			<tr>
				<td>Contact No.:</td>
				<td><?php echo odbc_result($resultHandle, "contactNo"); ?></td>
			</tr>
			<tr>
				<td>Job description:</td>
				<td><?php echo odbc_result($resultHandle, "jobDescription"); ?></td>
			</tr>
			<tr>
				<td>Salary:</td>
				<td><?php echo odbc_result($resultHandle, "salary"); ?></td>
			</tr>
			<tr>
				<td>Date hired:</td>
				<td>$<?php echo odbc_result($resultHandle, "dateHired"); ?></td>
			</tr>
			<tr>
				<td>Last login date:</td>
				<td><?php echo odbc_result($resultHandle, "lastLoginDate"); ?></td>
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
	<div id="contentBottomBrace"><img src="../images/jsc-v01_11.gif" alt="" /></div>
</div>

<div id="copyrightFooter">
	<span class="copyright">&copy; 2012 Joanna's Internet Cafe. All rights reserved.</span>
</div>

</body>

</html>

