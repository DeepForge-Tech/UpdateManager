#!/bin/bash
echo "==> Installing libraries"
# Installing libraries
YUM_PACKAGE_NAME="make cmake gcc-c++ curl libcurl sqlite-devel openssl-devel"
DEB_PACKAGE_NAME="g++ gcc build-essential cmake make curl libcurl4-openssl-dev libjsoncpp-dev libfmt-dev libsqlite3-dev libgtest-dev googletest google-mock libgmock-dev libtbb-dev libzip-dev zlib1g-dev"
PACMAN_PACKAGE_NAME="jsoncpp gcc base-devel cmake  clang gtest lib32-curl libcurl-compat libcurl-gnutls curl fmt lib32-sqlite sqlite sqlite-tcl zlib"
ZYPPER_PACKAGE_NAME="libcurl-devel gcc-c++ cmake gtest gmock zlib-devel fmt-devel sqlite3-devel jsoncpp-devel"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
   if [ -f /etc/os-release ]; then
      # freedesktop.org and systemd
      . /etc/os-release
      OS=$NAME
   elif type lsb_release >/dev/null 2>&1; then
      # linuxbase.org
      OS=$(lsb_release -si)
   elif [ -f /etc/lsb-release ]; then
      # For some versions of Debian/Ubuntu without lsb_release command
      . /etc/lsb-release
      OS=$DISTRIB_ID
   elif [ -f /etc/debian_version ]; then
      # Older Debian/Ubuntu/etc.
      OS=Debian
   elif [ -f /etc/SuSe-release ]; then
      # Older SuSE/etc.
      ...
      OS="openSUSE"
   elif [ -f /etc/redhat-release ]; then
      # Older Red Hat, CentOS, etc.
      ...
   else
      # Fall back to uname, e.g. "Linux <version>", also works for BSD, etc.
      OS=$(uname -s)
   fi
   if [[ "$OS" == "CentOS"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*
      sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*
      yum -y update
      yum -y install sudo
      sudo yum update -y
      sudo yum -y group install "Development Tools"
      sudo yum -y install $YUM_PACKAGE_NAME
   elif [[ "$OS" == "Red"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      yum -y update
      yum -y install sudo
      sudo yum update -y
      sudo yum -y group install "Development Tools"
      sudo yum -y install $YUM_PACKAGE_NAME
   elif [[ "$OS" == "Fedora"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      yum -y update
      yum -y install sudo
      sudo yum update -y
      sudo yum -y group install "Development Tools"
      sudo yum -y install $YUM_PACKAGE_NAME
   elif [[ "$OS" == "Ubuntu"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Debian"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Mint"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Knoppix"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "Manjaro Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo pacman -Sy jsoncpp --noconfirm
      sudo pacman -Sy gcc --noconfirm
      sudo pacman -Sy base-devel --noconfirm
      sudo pacman -Sy cmake --noconfirm
      sudo pacman -Sy clang --noconfirm
      sudo pacman -Sy gtest --noconfirm
      sudo pacman -Sy lib32-curl --noconfirm
      sudo pacman -Sy libcurl-compat --noconfirm
      sudo pacman -Sy libcurl-gnutls --noconfirm
      sudo pacman -Sy curl --noconfirm
      sudo pacman -Sy fmt --noconfirm
      sudo pacman -Sy lib32-sqlite --noconfirm
      sudo pacman -Sy sqlite --noconfirm
      sudo pacman -Sy sqlite-tcl --noconfirm
      sudo pacman -Sy zlib --noconfirm
      sudo pacman -Sy openssl-1.1 --noconfirm
      sudo pacman -Sy lib32-openssl --noconfirm
      sudo pacman -Sy libressl --noconfirm
   elif [[ "$OS" == "Kali GNU/Linux"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      sudo add-apt-repository universe
      sudo apt-get update
      sudo apt-get install -y $DEB_PACKAGE_NAME
   elif [[ "$OS" == "openSUSE Leap"* ]]; then
      echo "================================================"
      echo "Installing libraries"
      echo "================================================"
      zypper update -y
      zypper install sudo -y
      sudo zypper update -y
      sudo zypper install $ZYPPER_PACKAGE_NAME -y
   else
      echo "Not found package manager"
      exit 1;
   fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
   # Mac OSX
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   brew  install jsoncpp sqlite3 sqlite-utils fmt clang-format curl googletest gcc zlib cmake openssl && brew install gcc@7 libzip llvm
fi
echo "==> Libraries successfully installed"
esac
echo "=================================="
exit 0