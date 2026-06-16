<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<?php

	$haveErrors = 0;
	$errorString = "";

	// Ensure that we have some semblance of a sane call environment.
	if (!isset($_REQUEST['table']) || !isset($_REQUEST['form']))
	{
		$haveErrors = 1;
		$errorString = "This page may not be accessed as a silo.<br />" .
			 "Please access this page from one of the table " .
			 "management menus.<br />";
	}
	else
	{
		// Ensure that a valid page is to be included.
		switch ($_REQUEST['table'])
		{
		case 'customer':
		case 'employee':
		case 'payment':
		case 'pc':
		case 'session':
			break;

		default:
			$haveErrors = 2;
			$errorString = "Requested table \"" . $_REQUEST['table'] . "\" is invalid " .
				"input for this page.<br />";

			break;
		};

		if (!$haveErrors)
		{
			switch ($_REQUEST['form'])
			{
			case 'Add':
			case 'Remove':
			case 'List':
			case 'Edit':
			case 'Search':
			case 'Report':
				break;

			default:
				$haveErrors = 3;
				$errorString = "Requested form \"" . $_REQUEST['form'] . "\" is invalid " .
					"input for this page.<br />";

				break;
			};
		};
	};

	if (isset($_REQUEST['mode']))
	{
		switch ($_REQUEST['mode'])
		{
		case 'view':
		case 'details':
		case 'remove':
		case 'edit':
			break;

		default:
			$haveErrors = 4;
			$errorString = "Requested form mode \"" . $_REQUEST['mode'] . "\" is invalid.<br />";

			break;
		};
	};
?>

<html>

<head>
	<title>Forms :: <?php print $_REQUEST['table'] . $_REQUEST['form'] ?> form :: Timeless</title>
	<link rel="stylesheet" type="text/css" href="layout.css" />
	<link rel="icon" type="image/ico" href="images/favicon.ico" />
</head>

<body>

<div id="topBanner"><img src="images/jsc-v01_02.gif" alt=""  width="100%" /></div>
<div id="addrBar"><img src="images/jsc-v01_04.png" alt="" /></div>
<div id="topMenu">
	<?php require('menus/topMenu.php'); ?>
</div>

<div id="contentWrapper">
	<div id="contentTopBrace"><img src="images/jsc-v01_09.gif" alt="" /></div>
	<table id="contentDivider">
	<tr>
	<td id="menuBar">
		<div id="dbTableMenu"><?php require('menus/dbTableMenu.php'); ?></div>
	</td>
	<td id="contentBar">
		<div id="contentMain">
		<?php
			if ($haveErrors != 1) {
				require("forms/" . $_REQUEST['table'] . $_REQUEST['form'] . ".php");
			}
			else {
				print $errorString;
			};
		?>
		</div>
	</td>
	</tr>
	</table>
	<div id="contentBottomBrace"><img src="images/jsc-v01_11.gif" alt="" /></div>
</div>

<div id="copyrightFooter">
	<span class="copyright">&copy; 2012 Joanna's Internet Cafe. All rights reserved.</span>
</div>

</body>

</html>

