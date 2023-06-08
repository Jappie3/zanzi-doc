## Autossh Documentation: Creating Persistent SSH Tunnels

# Overview

AutoSSH is a tool that enables creating persistent SSH tunnels between machines, providing a secure and reliable way to access remote resources. This document explains how to use AutoSSH to create a tunnel from the gateway to the Hetzner machine and then over that tunnel, another tunnel that allows accessing the gateway from the Hetzner machine, even if the gateway's IP address changes.

### Installation

To install AutoSSH, execute the following command: `sudo apt install autossh`

### SSH Key Generation

Before using AutoSSH, it is necessary to generate SSH keys on the Hetzner and gateway machines. To generate SSH keys, execute the `ssh-keygen` command on both machines.

### Copying Public Key

After generating SSH keys, copy the public key from the gateway to the Hetzner VM and vice versa using the following command on the gateway and on the server:

`ssh-copy-id -i ~/.ssh/id_rsa.pub YOUR_USER_NAME@IP_ADDRESS_OF_THE_SERVER`
`ssh-copy-id -i ~/.ssh/id_rsa.pub YOUR_USER_NAME@IP_ADDRESS_OF_THE_GATEWAY`

### AutoSSH Configuration

To configure AutoSSH, create a systemd service unit file named `/etc/systemd/system/autossh.service` and paste the following bash script in it:

```bash  
# /etc/systemd/system/autossh.service  
[Unit]  
Description=AutoSSH, keeps tunnel to Hetzner VM open    
After=network-online.target ssh.service
		
[Service]  
User=flwsb  #local user of the machine  
# amount of seconds to wait before restarting  
RestartSec=16  
# restart service on failure  
Restart=on-failure  
# -p [PORT]  
# -l [user]  
# -M 0 --> no monitoring  
# -N Just open the connection and do nothing (not interactive)  
# -T -> disable pseudo-terminal allocation  
# -q -> quiet mode (suppress most warnings and diagnostics)  
# LOCALPORT:IP_ON_EXAMPLE_COM:PORT_ON_EXAMPLE_COM  
ExecStart=/usr/bin/autossh -M 0 -NT -q -o "StrictHostKeyChecking no" -o "ServerAliveInterval 60" -o "ServerAliveCountMax 3" -p 22 -l flwsb 94.130.179.123 -R 6600:127.0.0.1:22 -i /home/flwsb/.ssh/id_rsa

[Install]  
WantedBy=multi-user.target  
```
Make sure to change the username=flwsb to the username you have and the directory /home/flwsb/.ssh/id_rsa!


### Enabling and Starting AutoSSH

To enable and start AutoSSH, execute the following commands:
`sudo systemctl enable autossh; sudo systemctl start autossh`

### Updating AutoSSH Configuration

If you make changes to the AutoSSH configuration, execute the following commands:
`sudo systemctl daemon-reload; sudo systemctl restart autossh`

### Connecting to the Gateway

After the connection is established, connect to the Gateway from the Hetzner VM, use the following command:
Gateway Susa: `ssh -p 6600 flwsb@localhost -o StrictHostKeyChecking=no `
Gateway Kist: `ssh -p 6700 pi@localhost -o StrictHostKeyChecking=no `

By following the above steps, you can create a persistent SSH tunnel using AutoSSH, allowing you to access remote resources securely and reliably.