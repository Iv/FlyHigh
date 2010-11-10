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

%if 0%{?suse_version}
BuildRequires: libqt4-devel cmake update-desktop-files
Requires:      gnuplot update-desktop-files
%endif

%if 0%{?fedora}
BuildRequires: libqt4-devel cmake gcc-c++ desktop-file-utils
Requires:      gnuplot
%endif

%if 0%{?rhel_version} || 0%{?centos_version}
BuildRequires: qt4 qt4-devel cmake gcc-c++ desktop-file-utils
Requires:      gnuplot
%endif

%if 0%{?mandriva_version}
BuildRequires: libqt4-devel cmake
Requires:      gnuplot
%endif

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

%if 0%{?mandriva_version}  
export PATH=/usr/lib/qt4/bin:$PATH  
export QTDIR=%{_prefix}/lib/qt4/  
%endif

%if 0%{?rhel_version} || 0%{?centos_version}  
cmake -DQT_QMAKE_EXECUTABLE=/usr/lib/qt4/bin/qmake ..
%else
cmake ..
%endif
make %{?_smp_mflags}


%install
# install files into BuildRoot
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
# strip executable
strip -o $RPM_BUILD_ROOT/%{_bindir}/%{name} build/src/%{name}
# copy icon
mkdir -p $RPM_BUILD_ROOT/%_datadir/pixmaps/
cp src/images/flyhigh.png $RPM_BUILD_ROOT/%{_datadir}/pixmaps/%{name}.png


# proper position in the start menu
%if 0%{?suse_version}
%suse_update_desktop_file -c %{name} %{name} "Flight device & flight book manager" %{name} %{name} Graphics Database Archiving Utility Viewer
%endif

%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
# create a desktop file
mkdir -p $RPM_BUILD_ROOT/%_datadir/applications/
echo '[Desktop Entry]' > $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Encoding=UTF-8' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Name=flyhigh' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Comment=Flight device and flight book manager' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Exec=flyhigh' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Icon=flyhigh' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Type=Application' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Categories=Graphics;Archiving;Utility;Viewer;' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop

desktop-file-install --add-category="Database" --delete-original --dir=$RPM_BUILD_ROOT%{_datadir}/applications $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
%endif

%if 0%{?mandriva_version}  
# create a desktop file
mkdir -p $RPM_BUILD_ROOT/%_datadir/applications/
echo '[Desktop Entry]' > $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Encoding=UTF-8' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Name=flyhigh' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Comment=Flight device and flight book manager' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Exec=flyhigh' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Icon=flyhigh' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Type=Application' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop
echo 'Categories=Graphics;Archiving;Utility;Viewer;' >> $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop

%{update_menus}
%endif


%clean
# clean up the hard disc after build
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc AUTHORS COPYING README INSTALL TODO
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png



%changelog
* Tue Nov 09 2010 - ja_kern@sf.net
- support for mandriva

* Mon Nov 08 2010 - ja_kern@sf.net
- support for fedora, rhel and centos

* Mon Oct 11 2010 - ja_kern@sf.net
- initial package created
