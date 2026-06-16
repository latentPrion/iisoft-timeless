<?php
	if (!isset($haveErrors))
	{
		die("Please access this page through one of the table " .
			"management links.<br />");
	};

	if ($haveErrors) {
		die($errorString);
	};

	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle) {
		die ("Failed to connect to DB!<br />");
	};

	$queryString = "SELECT * FROM customer";
	$customerHandle = odbc_exec($dbHandle, $queryString);
?>

<form method="post" action="../processing/customerReport.php">
<span class="normal">
<table width="100%">
<?php
	while (odbc_fetch_row($customerHandle))
	{
?>
	<tr><td colspan="4">Customer Report</td></tr>
	<tr><td colspan="4"><hr width="100%" /></td></tr>
	<tr>
		<td>ID:</td>
		<td><?php echo odbc_result($customerHandle, "id"); ?></td>
		<td>Username:</td>
		<td><?php echo odbc_result($customerHandle, "username"); ?></td>
	</tr>
	<tr>
		<td>Name:</td>
		<td><?php echo odbc_result($customerHandle, "fullName"); ?></td>
		<td>Email:</td>
		<td><?php echo odbc_result($customerHandle, "email"); ?></td>
	</tr>
	<tr>
		<td colspan="1">Address:</td>
		<td colspan="4"><?php echo odbc_result($customerHandle, "address"); ?></td>
	</tr>
	<tr>
		<td>Time Remaining:</td>
		<td><?php echo odbc_result($customerHandle, "timeRemaining"); ?></td>
		<td>Phone contact:</td>
		<td><?php echo odbc_result($customerHandle, "contactNo"); ?></td>
	</tr>
	<tr>
		<td colspan="2">Join date:</td>
		<td colspan="4"><?php echo odbc_result($customerHandle, "joinDate"); ?></td>
	</tr>
<?php
	};
	odbc_close($dbHandle);
?>
</table>
</span>
</form>

