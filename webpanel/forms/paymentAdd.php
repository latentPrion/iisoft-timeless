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

<form method="post" action="../processing/paymentAdd.php">
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
		default: echo "Insertion of new payment was successful!";
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
	<td>Customer name:</td>
	<td><select><option>--Choose a customer from the list --</option></select></td>
	</tr>
	<tr>
	<td>Amount:</td>
	<td><input type="text" maxlength="64" name="payment_email" size="50" /></td>
	</tr>
	<tr><td></td></tr>
	<tr>
	<td colspan="2"><input type="submit" value="Add new payment" /></td>
	</tr>
</table>
</form>

