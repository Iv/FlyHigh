Name:          flyhigh
License:       GPL
Group:         Productivity/Databases/Clients
Summary:       A GPS flight device manager
Version:       0.7.0
Release:       1
URL:           http://flyhigh.sourceforge.net/
BuildRoot:     %{_tmppath}/%{name}-%{version}-build
Source0:       %name-%{version}.tar.gz

Provides:      flyhigh = %{version}
Obsoletes:     flyhigh < %{version}


BuildRequires: libqt4-devel cmake update-desktop-files
Requires:      gnuplot update-desktop-files



%description
Configuration of GPS flight devices, r/d tasks, r/w/d waypoints, r/w/d routes,
r/w/d CTRs, flight book. Show flight as graphic using gnuplot.
Supported devices: Flytec 5020, 6015, Competino and IQ Basic from Brauniger.


%prep
# extract the source and go into the flyhigh-0.7.0 directory
%setup -q

%build
# generate makefiles and build
mkdir build
cd build
cmake ..
make %{?_smp_mflags}


%install
# install files into BuildRoot
mkdir -p $RPM_BUILD_ROOT/usr/bin/
# strip executable
strip -o $RPM_BUILD_ROOT/usr/bin/flyhigh build/src/flyhigh
# copy icon
mkdir -p $RPM_BUILD_ROOT/usr/share/pixmaps/
cp src/images/flyhigh.png $RPM_BUILD_ROOT/usr/share/pixmaps/%name.png

# would need some tweaking of the make install target:
#cd build
#%makeinstall
#cd ..

# proper position in the KDE menu
%suse_update_desktop_file -c %name %name "Flight device & flight book manager" %name %name Graphics Database Archiving Utility Viewer


%clean
# clean up the hard disc after build
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc AUTHORS COPYING README INSTALL TODO
%{_bindir}/flyhigh
/usr/share/applications/%name.desktop
/usr/share/pixmaps/%name.png



%changelog
* Mon Oct 11 2010 - ja_kern@sf.net
- initial package created
