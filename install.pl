use File::Path;
use File::Copy;

#get perl path
foreach (split(/;/, $ENV{'path'}))
{
	$PerlBin = $_,
	last
		if $_ =~ /perl\\bin$/i;
}

$PerlBin =~ s#\\#/#g;

#get perl version
$BUILD = $] eq '5.00307' ? '3xx' : ($] eq '5.00502' || $] eq '5.00503') ? '5xx' : $] eq 5.006 ? '6xx' : 'xxx';

die "Wrong build number ..."
	if $BUILD ne '6xx' && $BUILD ne '5xx' && $BUILD ne '3xx';

#build lib path
$INST_LIBDIR = ($BUILD eq '6xx' || $BUILD eq '5xx') ? "$PerlBin/../site/lib/" : "$PerlBin/../lib/";

-d $INST_LIBDIR or die "Cannot find library directory ...";

copy('Lanman.pm', $INST_LIBDIR . 'Win32/Lanman.pm');

mkpath($INST_LIBDIR . 'auto/Win32/Lanman');

if($BUILD eq '6xx')
{
	copy('Lanman.6xx.dll', $INST_LIBDIR . 'auto/Win32/Lanman/Lanman.dll');
}
elsif($BUILD eq '5xx')
{
	copy('Lanman.5xx.dll', $INST_LIBDIR . 'auto/Win32/Lanman/Lanman.dll');
}
else
{
	copy('Lanman.3xx.pll', $INST_LIBDIR . 'auto/Win32/Lanman/Lanman.pll');
}
