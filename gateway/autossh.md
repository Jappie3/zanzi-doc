# Autossh

- We chose to set up autossh on the LoRaWAN gateways so that we could manage & update them if necessary
    - We cannot use normal SSH, since there are no port-forwarding rules set on the routers of the networks the gateways are on
    - This means that we cannot reach the gateways via the internet. A solution to this is to have the gateways set up the connection to the server (which we can reach), and then use that connection to set up a second connection to the gateway.
- What does this mean in practice:
    - As soon as the gateways boot, the autossh program will start (more on this later)
    - This is the command we used: `/usr/bin/autossh -M 0 -NT -q -o "StrictHostKeyChecking no" -o "ServerAliveInterval 60" -o "ServerAliveCountMax 3" -p 22 -l <user> <server_ip> -R 6600:127.0.0.1:22 -i /home/<user>/.ssh/id_rsa`
        - `-M` -> no monitoring port
        - `-N` -> do not execute a remote command (only forward the port)
        - `-T` -> disable pseudo-terminal allocation
        - `-q` -> quiet mode (suppress most warnings & diagnostics)
        - `-o` -> set option (see `man ssh`)
        - `-p` -> port to connect to on the remote host
        - `-l` -> login name
        - `-R` -> connections to the given port on the remote server should be forwarded to the local side
            - In this case: connections to `6600` on the remote server (the backend) should be forwarded to `127.0.0.1:22` (the local SSH port)
            - Note: if you want to set up autossh on multiple gateways, **the port on the remote server should differ** (only one process can listen on a port)
        - `-i` -> identity file (private key) to use for the connection
        - More information about all these options: `man ssh` & `man autossh`
- Setting up autossh:
    - First, make sure that the public key of the gateway is in the `~/.ssh/authorized_keys` file of the server and that the public key of the server is in the `~/.ssh/authorized_keys` file of the gateway
    - Then, install autossh via your favourite package manager (e.g. `sudo apt install autossh`)
    - To enable autossh on startup, we created a `systemd` service (see below)
        - Enable & start the service: `sudo systemctl enable --now autossh`
        - To check the status of the service: `systemctl status autossh`
        - When you made changes to the file, make sure to run `sudo systemctl daemon-reload && sudo systemctl restart autossh`

```bash
# put this file in /etc/systemd/system/autossh.service
[Unit]
Description=AutoSSH, keeps tunnel to Hetzner VM open  
After=network-online.target ssh.service

[Service]
User=flwsb # change this to the local user
# amount of seconds to wait before restarting
RestartSec=16
# restart service on failure
Restart=on-failure
# command:
ExecStart=/usr/bin/autossh -M 0 -NT -q -o "StrictHostKeyChecking no" -o "ServerAliveInterval 60" -o "ServerAliveCountMax 3" -p 22 -l <server_user> <server_ip> -R 6600:127.0.0.1:22 -i /home/<user>/.ssh/id_rsa
# make sure to replace <server_user>, <server_ip> and <user> with the correct values in the line above (server_user = user on the server to which you want to set up an SSH connection, user = local user on the gateway)

[Install]
WantedBy=multi-user.target
```

- Now, every time the gateway boots, an SSH tunnel will be created. This tunnel can be used to SSH into the gateway: `ssh -o StrictHostKeyChecking=no -p 6600 <user>@localhost`
    - `<user>` is the user on the gateway
    - The `StrictHostKeyChecking` option is required only when you have multiple tunnels going to the server, since that would mean you have different 'fingerprints' on the same host (`localhost`), which triggers a warning without that option
