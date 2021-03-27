# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "generic/ubuntu2004"
  config.vm.synced_folder ".", "/src"

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y \
      bison \
      build-essential \
      clang \
      cmake \
      flex \
      llvm-11-dev \
      ninja-build \
      zlib1g-dev
  SHELL
end
