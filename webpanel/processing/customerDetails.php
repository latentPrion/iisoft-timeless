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
		. "FROM customer "
/*			. "INNER JOIN employee "
			. "ON customer.employeeId = employee.id " */
		. "WHERE customer.id = " . $_REQUEST['id']);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>

<head>
	<title>Customer #<?php echo $_REQUEST['id'] ?> :: Details :: Timeless</title>
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
				<td>Username:</td>
				<td><?php echo odbc_result($resultHandle, "username"); ?></td>
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
				<td>Time remaining:</td>
				<td><?php echo odbc_result($resultHandle, "timeRemaining"); ?> min</td>
			</tr>
			<tr>
				<td>Subscribed?:</td>
				<td><?php echo ((odbc_result($resultHandle, "subscribed") == 0) ? "No" : "Yes") ; ?></td>
			</tr>
<?php
		if (odbc_result($resultHandle, "subscribed") == 1)
		{
?>
			<tr>
				<td>Subcription Type:</td>
				<td><?php echo odbc_result($resultHandle, "subscriptionType"); ?></td>
			</tr>
			<tr>
				<td>Subcription valid time duration:</td>
				<td>
					<?php
						echo ((odbc_result($resultHandle, "subscriptionMaxTime") == 0)
							? "Unlimited until end date" : odbc_result($resultHandle, "subscriptionMaxTime"));
					?>
				</td>
			</tr>
			<tr>
				<td>Subscription start date:</td>
				<td><?php echo odbc_result($resultHandle, "subscriptionStartDate"); ?></td>
			</tr>
			<tr>
				<td>Subscription end date:</td>
				<td><?php echo odbc_result($resultHandle, "subscriptionEndDate"); ?></td>
			</tr>
<?php
		};
?>
			<tr>
				<td>Total amount spent by this customer:</td>
				<td>$<?php echo odbc_result($resultHandle, "totalAmountSpent"); ?></td>
			</tr>
			<tr>
				<td>Last login date:</td>
				<td><?php echo odbc_result($resultHandle, "lastLoginDate"); ?></td>
			</tr>
			<tr>
				<td>Customer join date:</td>
				<td><?php echo odbc_result($resultHandle, "joinDate"); ?></td>
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

