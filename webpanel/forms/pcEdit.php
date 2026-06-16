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

Please access this page through the <a href="form.php?table=pc&form=List&mode=edit">Edit pc</a> form.

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
		. "FROM pc "
		. "WHERE pc.id = " . $_REQUEST['id'];

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

<form action="processing/pcEdit.php" method="post">
<table>
	<tr>
		<td colspan="2">Editing pc #<?php echo $_REQUEST['id'] . "."; ?></td>
	</tr>
	<tr>
		<td>Purchase date:</td>
		<td><input type="text" name="edit_purchaseDate" value="<?php echo odbc_result($resultHandle, 'purchaseDate'); ?>" /></td>
	</tr>
	<tr>
		<td>Description:</td>
		<td><input type="text" name="edit_description" value="<?php echo odbc_result($resultHandle, 'description'); ?>" /></td>
	</tr>
	<tr>
		<td colspan="2">
			<input type="submit" value="Edit pc" />
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

