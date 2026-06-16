<?php
	if (!isset($haveErrors))
	{
		die("Please access this page through one of the table " .
			"management links.<br />");
	};

	if ($haveErrors) {
		die($errorString);
	};
?>

<form method="post" action="../processing/customerAdd.php">
<table>
<?php
	if (isset($_REQUEST['error']))
	{
?>
	<tr>
		<td colspan="2" style="background: <?php if ($_REQUEST['error'] != 0) { echo '#cc7777'; } else { echo '#77cc77'; }; ?>;">
<?php
		switch ($_REQUEST['error'])
		{
		default: echo "Insertion of new customer was successful!";
			break;

		case 1: echo "Please do not attempt to access the back end page directly. Use this form.";
			break;

		case 2: echo "Connection to database was unsuccessful.";
			break;

		case 3: echo "Insert query for the new record failed.";
			break;
		};
?>
		</td>
	</tr>
<?php
	};
?>
	<tr>
	<td>Username:</td>
	<td><input type="text" maxlength="64" name="customer_username" size="50" /></td>
	</tr>
	<tr>
	<td>Password:</td>
	<td><input type="password" maxlength="64" name="customer_password" size="50" /></td>
	</tr>
	<tr>
	<td>Customer name:</td>
	<td><input type="text" maxlength="64" name="customer_fullName" size="50" /></td>
	</tr>
	<tr>
	<td>Customer email:</td>
	<td><input type="text" maxlength="64" name="customer_email" size="50" /></td>
	</tr>
	<tr>
	<td>Customer address:</td>
	<td><input type="" maxlength="64" name="customer_address" size="50" /></td>
	</tr>
	<tr>
	<td>Contact number:</td>
	<td><input type="" maxlength="64" name="customer_contactNo" size="50" /></td>
	</tr>
	<tr>
	<td>Customer join date:</td>
	<td><input type="" maxlength="64" name="customer_joinDate" size="50"
		value="<?php echo @date('Y-m-d '); echo @strftime('%H:%M:%S'); ?>"/></td>
	</tr>
	<tr><td></td></tr>
	<tr>
	<td colspan="2"><input type="submit" value="Add new customer" /></td>
	</tr>
</table>
</form>

