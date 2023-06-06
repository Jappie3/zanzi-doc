# Gateway Installation

## Important notices

There are a couple of things you should not do when the board is powered, as they may break the board.
- Plug / unplug any cable, including USB
- Leaving the LoRaWAN antenna cables unattached


## Glossary

- **Ansible**, Configuration automation tool
- **RTL433**, Software for listening to the weatherstation packets
- **UFW**, Linux firewall
- **Wireguard**, VPN software

## Installation

### 1. Preparing the raspberry pi board

We start with placing the compute module on the gateway board, we then place the batteries (ICR18650-26J) for the UPS on the board (take care of orientation!) and a CR2032 battery for the clock on the back of the board.

If not sure look on the wiki site of this board [CM4-POE-UPS-BASE - Waveshare Wiki](https://www.waveshare.com/wiki/CM4-POE-UPS-BASE). You may then provide the board of power with either a dc powersupply or with POE. After these steps on should flip the boot switch to on. ![bootswitch](bootswitch.png)

To flash an os on the board we use [Rpiboot](https://www.waveshare.com/w/upload/f/f3/Rpiboot_setup.zip) from waveshare. After downloading, unzipping and installing this program you can plug a usb cable from your computer to the rapsberry pi board. Then you switch the slider to power the board. After this you run Rpiboot. We then proceed using the Raspberry Pi Imager which can be downloaded from the official raspberry pi site: [https://www.raspberrypi.com/software/](https://www.raspberrypi.com/software/). The project is made for the following OS choice: Raspberry Pi OS Lite (64-bit). You know select the compute module and click on the settings icon in the down right corner, here we set a hostname, enable SSH and change the user credentials.
Next you click on write and wait until it is finished. When its finished you can unplug the usb-c connection and power it off. Then you turn the boot switch back to its original position.

Now we need to put the pi on a wired network with DHCP so it automaticly gets an ip address.
It is advisee to setup this ip staticly on the first ssh session.

After these settings your Raspberry pi compute board should have a working base setup and should work when powered by POE.

### 2. Ansible playbook

We will use [Ansible](https://www.ansible.com/) to further configure the OS we just installed.
To install Ansible on your host see official documentation on: [docs.ansible.com](https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html)
 The script will provide the following changes.

1. Update and upgrade your gateway to the latest version
2. Enable SPI and I²C
3. Install, configure and enable UFW
4. Download and setup sshkeys for useraccess over ssh from Github 
5. Remove ssh password login

To start this script we require a Linux or MacOS host that has ansible installed, this host should have access to the PI over the network using ssh.

Ensure the following files are on the ansible host:
- ansible.cfg - sets up ansible work environment
- hosts - contains the rapsberry pi connection details
- gateway.yml - playbook that configures the host

Change the host connection details in the hosts file and set list of Github usernames in the gateway.yml file under the vars: usernames to the user handles of the users you would like to give access over ssh. To add a ssh key to github refer to the official documentation  ([Adding a new SSH key to your GitHub account - GitHub Docs](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)). Make sure you add the SSH key of your Linux machine and of all the machines you want to acces the gateway with.
Now you can run the playbook from ansible host with the next command: 
```shell
ansible-playbook gateway.yml
```

### 3. Configure services

To setup all the configuration for our services we need to run a interactive bash script. Before running this script make sure that you have created a [The Things Network](https://www.thethingsnetwork.org/) gateway and have access to its configuration, this gateway wil be required during this script. This script should be run remotely over ssh on the Raspberry Pi as root, we can accomplish this using the following bash commands after the file has been placed on the PI.

```shell
sudo chmod +x installGateway.sh
sudo ./installGateway.sh
```

This script will go through the next steps.

1. Install all the necessary software (for the gateway, wireguard and rtl-433).
2. Clone a github repository that we will use for the LoRaWAN gateway (We used the following tutorial as a basemodel [Building a gateway with Raspberry Pi and IC880A | The Things Stack for LoRaWAN (thethingsindustries.com)](https://www.thethingsindustries.com/docs/gateways/models/raspberry-pi/)).
3. Ask to copy and secure the EUI that the script gives you, this is required later.
4. Ask for the [The Things Network](https://www.thethingsnetwork.org/) gateway API key. Follow this tutorial on how to make an API key. [Create an API Key](https://www.thethingsindustries.com/docs/gateways/concepts/lora-basics-station/lns/#create-an-api-key)
5. Setup the LoRaWan gateway and create the corresponding linux services
6. Ask for the Moquitto MQTT server credentials
7. Setup the rtl_433 program and create the corresponding linux services
8. Ask for the Wireguard configuration
9. Setup Wireguard and create the corresponding linux services
10. Reboot the gateway

After these steps the gateway should be up and running.