<?php
	if (!isset($haveErrors))
	{
		die("Please access this page through one of the table " .
			"management links.<br />");
	};

	if ($haveErrors) {
		die($errorString);
	};

	if (!isset($_REQUEST['id']))
	{
?>

Please access this page through the <a href="form.php?table=payment&form=List&mode=edit">Edit payment</a> form.

<?php
		die();
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

Failed to connect to the database!

<?php
		die();
	};

	$queryString = "SELECT * "
		. "FROM payment INNER JOIN customer ON payment.customerId = customer.id "
		. "WHERE payment.id = " . $_REQUEST['id'];

	$resultHandle = @odbc_exec($dbHandle, $queryString);
	if (!$resultHandle)
	{
?>

Query failed!

<?php
		die();
	};

	if (odbc_fetch_row($resultHandle))
	{
?>

<form action="processing/paymentEdit.php" method="post">
<table>
	<tr>
		<td colspan="2">Editing payment #<?php echo $_REQUEST['id'] . ": from \"" . odbc_result($resultHandle, "fullName") . "\""; ?></td>
	</tr>
	<tr>
		<td>Customer Name:</td>
		<td><input type="text" name="edit_fullName" value="<?php echo odbc_result($resultHandle, 'fullName'); ?>" /></td>
	</tr>
	<tr>
		<td>Amount:</td>
		<td><input type="text" name="edit_email" value="<?php echo odbc_result($resultHandle, 'amount'); ?>" /></td>
	</tr>
	<tr>
		<td colspan="2">
			<input type="submit" value="Edit payment" />
			<input type="hidden" name="edit_valid" value="1" />
			<input type="hidden" name="edit_id" value="<?php echo $_REQUEST['id']; ?>" />
		</td>
	</tr>
</table>
</form>

<?php
	};

	odbc_close($dbHandle);
?>

