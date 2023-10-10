# Installation process

## 1. Preparation

### Server

- Make sure you have a server somewhere, this can be a VM (virtual machine) on your local PC or in the cloud

- I highly recommend to 1) not use the root user and 2) logging in with SSH keys instead of a password
	- Making a new user:
		- `adduser flwsb` -> this makes a new user `flwsb`
		- You can leave all the options blank (just press enter) except for the password (make sure to choose a **strong password**)
	- Using SSH keys:
		- If you don't have an SSH key you can **generate** one with the `ssh-keygen` command (consult the man page - `man ssh-keygen` - or the internet for more information)
		- Then, **copy** your SSH key to the server: `ssh-copy-id <user>@<ip>` (`<user>` = user on the VM, `<ip>` = IP of the VM)
			- Example: `ssh-copy-id FLWSB@1.2.3.4`
			- This command will add the key to a file in the home directory of `<user>`, namely `~/.ssh/authorized_keys`
		- Now you should be able to **log in** like this: `ssh <user>@<ip>` (`<user>` = user on the VM, `<ip>` = IP of the VM)
			- Example: `ssh FLWSB@1.2.3..4`
- You can disable SSH login with password and only allow login with SSH keys (which is more secure) like this:

```bash
# open the file /etc/ssh/sshd.config with a text editor, for example nano, vim, etc.

# add the following lines somewhere in the file (it doesn't matter where exactly):
PubkeyAuthentication yes
PasswordAuthentication no

# run `sudo systemctl reload ssh` to reload the ssh configuration
```

### Files

- All the files necessary for installation are located in [this Git repository](https://github.com/Jappie3/flexible-lorawan-architecture)
	- Download the repository: `git clone https://github.com/Jappie3/flexible-lorawan-architecture`
- There are 2 files that you need to create & modify **before running the Ansible playbook:** `.env` and `hosts` (both in the `ansible/` directory)
	- `.env` contains things like passwords and usernames used in the Docker containers, this file may **NEVER** be included in git since it should stay secret
	- `hosts` contains the IP address of the server (this file is not included in git because I don't know where you want to run the playbook, thus I also can't know the IP)
- In `ansible/` there are 2 templates for the respective files as well as some examples


```bash
$ tree -L 4
.
├── ansible
│   ├── ansible.cfg # ansible config
│   ├── FLWSB-backend-ansible.yaml # ansible playboook
│   ├── hosts # hosts file (not included in git)
│   ├── hosts.default # template of hosts file
│   ├── .env # secrets file (not included in git)
│   └── .env.default # template of secrets file
├── README.md
└── src # contains docker compose file & configuration for all the containers
    ├── docker-compose.yaml
    ├── grafana
    │   └── datasources
    │       └── influxdb.yaml
    ├── influxdb
    │   ├── Dockerfile
    │   └── initBuckets.sh
    ├── mosquitto
    │   ├── Dockerfile
    │   ├── entrypoint-custom.sh
    │   └── mosquitto.conf
    ├── nginx
    │   └── nginx.conf
    └── nodered - DEPRECATED
        ├── data
        │   ├── bcrypt_hash.js
        │   ├── flows_cred.json
        │   ├── flows.json
        │   ├── lib
        │   ├── node_modules
        │   ├── package.json
        │   ├── package-lock.json
        │   └── settings.js
        ├── Dockerfile
        └── entrypoint-custom.sh
```

## 2. Some background on Ansible and Docker

- In [backend.md](./backend.md) you can read more about the applications that make up the backend, this part will go in detail about the installation and configuration of the server
- There is an Ansible playbook for configuring the server, which can be run with `ansible-playbook -K -i hosts FLWSB-backend-ansible.yaml`

### Ansible

- Ansible is a tool to configure servers via SSH
	- You can write all your configuration in a **playbook** and then run the playbook on **one or more servers**
		- If something goes catastrophically wrong, you can run the playbook again to install everything on the server
	- Ansible will then configure the servers so that their configurations **matches** the configuration of the playbook
- The default directory for the Ansible configuration on a Linux machine is `/etc/ansible/`
	- The hosts on which you want to run a playbook can be specified in a `hosts` file ([link to documentation](https://docs.ansible.com/ansible/latest/inventory_guide/intro_inventory.html#inventory-basics-formats-hosts-and-groups))
	- The configuration of Ansible is saved in `ansible.cfg`

```yaml
# example hosts file:
[someserver] # this is the name which you can use in your playbook
192.168.122.44 user=myusername # this is the ip of the machine, optionally followed by one or more variables that can be used in the playbook

# you can also just specify an IP address in your playbook instead of using a hosts file

---

# example ansible.cfg file:
[defaults]
inventory=/etc/ansible/hosts # if you use a hosts file, specify it here
```

- Ansible works with modules, for example the `ansible.builtin.apt` module can be used to install, remove or modify packages on a Debian system via `apt` ([link to documentation](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/apt_module.html#ansible-collections-ansible-builtin-apt-module))
	- There are modules for pretty much anything and you can even make your own if you really want to (but that's beyond the scope of this document)

- Some important things to note:
    - Ansible is **NOT** a backup of your data
    - Ansible will **NOT** restore things you manually changed after running the playbook
    - Ansible is **NOT** an excuse to have weak security
    - Ansible **IS** a tool to easily manage a lot of servers and to prevent repetitive work
    - Ansible **CAN** be used as a backup of your configuration (but **NOT** of your data)

- Also note that Ansible **only** needs to be installed on the **computer running the playbook**, and **not** on the **server**. The computer running the playbook will configure the server via the SSH protocol, so the server does **not** need Ansible.


- One last thing: There is a very useful tool called `ansible-lint` which you can use to check the syntax of your playbooks
    - E.g. `ansible-lint FLWSB-backend-ansible.yaml`
    - This will check your playbook for potential problems and also **link to the relevant documentation** (see 'Rule violation summary' at the bottom)

### The playbook

- A brief explanation of the first few lines of our playbook
	- If you want more information about a specific module I suggest you look it up online, usually they have very good documentation and also some examples
	- E.g. documentation of all the `ansible.builtin` modules on [docs.ansible.com](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html)

```yaml
---

- name: Hetzner server for FLWSB backend # name of the playbook
  hosts: FLWSB-hetzner # host(s) on which to run the playbook
  user: '{{ user }}' # use the `user` variable defined in the hosts file, the playbook will be run on the server as this user
  vars:
	domains: # set a variable called 'domains'
		- grafana.iot-ap.be
		- api.iot-ap.be
  tasks:

# RUN THIS PLAYBOOK: ansible-playbook -K -i ansible.cfg FLWSB-backend-ansible.yaml
# -K -> ask become password (root)
# -i -> specify ansible config file
# optional: add -v, -vv or -vvv to increase verbosity (more logs, handy when troubleshooting)

	- name: Update apt cache # name of the task
	ansible.builtin.apt: # module to use for this task
		update_cache: true # update the apt cache (equivalent of `sudo apt update`)
	become: true # execute this task as root

# ...
```

### Docker

- Docker is an open source platform for running containers
- A Docker container is a running image
	- An image is a 'snapshot' of a Docker application
	- You can find images for almost every application you can think of (databases, webservers, etc.) on [hub.docker.com](https://hub.docker.com/)
- For installing Docker, I suggest you follow the [official documentation](https://docs.docker.com/engine/install/) and pick the guide that suits your OS, e.g. [docs.docker.com/engine/install/debian](https://docs.docker.com/engine/install/debian/) for Debian
- Some Docker commands:
	- `docker ps -a` -> see all containers (omit `-a` -> only show running containers)
	- `docker run nginx` -> create and run a new container from an image, here `nginx`
	- `docker images` -> list all downloaded images
	- `docker pull nginx` -> download an image, here `nginx`
	- `docker logs my-nginx-container` -> see logs of the container `my-nginx-container`
	- `docker inspect my-nginx-container` -> see detailed information about `my-nginx-container`
- You can run containers with `docker run`, but you can also put the configuration for your containers in a file called `docker-compose.yaml`
	- This file can be executed with `docker compose up -d`
		- Make sure you are in the same directory as the `docker-compose.yaml` file
		- `-d` option -> detach, run containers in the background (so you can still use your terminal)
			- In some scenarios you can omit `-d` to see more logs
	- If you want to bring the containers down again: `docker compose down`
		- Make sure you are in the same directory as the `docker-compose.yaml` file

### The `docker-compose.yaml`

- Here is an excerpt of the Docker compose file with some extra comments

```yaml
version: "3"
services:
	nginx: # the name of the service
		image: nginx:latest # which image to use for the container
		container_name: flwsb-nginx # the name of the container
		volumes: # volume can be a path on the host that maps to a folder inside the container or a docker volume that maps to a folder inside the container
			- ./nginx/nginx.conf:/etc/nginx/conf.d/site.conf
			- /etc/letsencrypt/:/etc/letsencrypt/
		ports: # ports to forward from the host to the container
			- 80:80
			- 443:443
		restart: on-failure:10 # when to restart and how many times
```


> Note: There is no need to run `docker compose up -d` after running the Ansible playbook, since there is a task that does that

## 3. Configuration

- **Before running the playbook**, make sure to do the following:
    - Copy `.env.default` to `.env` (`cp .env.default .env`) and modify the set the usernames & passwords. You can find the credentials for The Things Network (TTN) on their website.
    - Copy `hosts.default` to `hosts`
        - `IP`: IP address of the server
        - `user`: a user on the server that is in the `sudoers` file
        - `email`: the e-mail address with which the certificate for the domain should be signed
    - In the playbook, under `vars` (line 7 & 8), change the domains to 2 domains that you own
        - Make sure to also change these domains in the `src/nginx/nginx.conf` file, lines 3, 9, 11, 12, 29, 31 & 32
- If the playbook ran successfully, everything should be up and running just like you configured it.
    - You can check the status of all the Docker containers on the server: `docker ps -a`
    - The Grafana dashboard should be accessible on the URL you configured (make sure to correctly set your DNS record(s) to point to the IP address of the server)
