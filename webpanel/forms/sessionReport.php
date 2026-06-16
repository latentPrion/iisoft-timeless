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

	$queryString = "SELECT * FROM session";
	$customerHandle = odbc_exec($dbHandle, $queryString);
?>

<form method="post" action="../processing/customerReport.php">
<span class="normal">
<table width="100%">
	<tr><td colspan="4">Customer Report</td></tr>
<?php
	while (odbc_fetch_row($customerHandle))
	{
?>
	<tr><td colspan="4"><hr width="100%" /></td></tr>
	<tr>
		<td>Session ID:</td>
		<td><?php echo odbc_result($customerHandle, "id"); ?></td>
		<td>Customer ID:</td>
		<td><?php echo odbc_result($customerHandle, "customerId"); ?></td>
	</tr>
	<tr>
		<td>Start Time:</td>
		<td><?php echo odbc_result($customerHandle, "startTime"); ?></td>
		<td>Session Type::</td>
		<td><?php echo odbc_result($customerHandle, "type"); ?></td>
	</tr>
	<tr>
		<td>PC ID:</td>
		<td><?php echo odbc_result($customerHandle, "pcId"); ?></td>
		<td>Session Type:</td>
		<td><?php
			if (odbc_result($customerHandle, "type") == 0) {
				echo "(Normal session)";
			} else {
				echo "(Temporary Session)";
			};
			?>
		</td>
	</tr>
<?php
		if (odbc_result($customerHandle, "type") == 1) 
		{
?>
	<tr>
		<td colspan="1">Time Remaining:</td>
		<td colspan="4"><?php echo odbc_result($customerHandle, "timeRemaining"); ?></td>
	</tr>
<?php
		};
	};
	odbc_close($dbHandle);
?>
</table>
</span>
</form>

