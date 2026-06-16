<?php
	if (!isset($haveErrors))
	{
		die("Please access this page through one of the table " .
			"management links.<br />");
	};

	if ($haveErrors) {
		die($errorString);
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle) {
		printf("ODBC connection failure; Reason: %s.<br />", odbc_error());
	};

	
?>

<form method="post" action="../processing/sessionView.php">
<span class="normal">
<?php
	$resultHandle = odbc_exec($dbHandle, "select * from session");
?>
<table style="width: 100%">
<?php
	if (isset($_REQUEST['error']))
	{
?>
	<tr>
		<td colspan="4" style="background: <?php
		switch ($_REQUEST['error'])
		{
		default: echo "#77cc77\">Record deleted successfully.";
			break;

		case 1:
			echo "#cc7777\">Invalid page access: Please use this page to select which records to delete.";
			break;

		case 2:
			echo "#cc7777\">Failed to connect to the database!";
			break;

		case 3:
			echo "#cc7777\">Database internal error, or record does not exist. Please close other programs which may be using the DB.";
			break;
		};
	};
?></td>
	</tr>
	<tr>
		<td colspan="4">Session listing.</td>
	</tr>
	<tr>
		<td>Id:</td>
		<td>Start time:</td>
		<td>Time Remaining:</td>
		<td></td>
	</tr>
<?php
	while (odbc_fetch_row($resultHandle))
	{
?>
	<tr>
		<td><?php echo odbc_result($resultHandle, "id"); ?></td>
		<td><?php echo odbc_result($resultHandle, "startTime"); ?></td>
		<td><?php echo odbc_result($resultHandle, "timeRemaining"); ?> min</td>
		<td>
			<?php
				switch ($_REQUEST['mode'])
				{
				default:
					echo "<a href=\"processing/sessionDetails.php?id=" . odbc_result($resultHandle, "id") . "\">View details</a>";
					break;

				case 'edit':
					echo "<a href=\"form.php?table=session&form=Edit&id=" . odbc_result($resultHandle, "id") . "\">Edit session</a>";
					break;

				case 'remove':
					echo "<a href=\"processing/sessionRemove.php?id=" . odbc_result($resultHandle, "id") . "\">Delete session</a>";
					break;
				};
			?>
		</td>
	</tr>
<?php
	};

	odbc_close($dbHandle);
?>
</table>
</span>
</form>

