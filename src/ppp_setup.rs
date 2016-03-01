use std::io;
use std::fs::{self, OpenOptions};
use std::os::unix::fs::OpenOptionsExt;
use std::io::Write;

fn get_content(username: &str, password: &str, net_interface: &str) -> String {
    let result = format!("plugin rp-pppoe.so
plugin libHubeiExin4PPP.so

# Network Interface
{net_interface}

debug
persist
defaultroute
hide-password
noauth
usepeerdns
nodetach
name {username}
pwd {password}", username = username, password = password, net_interface = net_interface);
    return result.to_string();
}

fn main() {
    println!("Hubei Exin Setup for PPP");
    let mut file = OpenOptions::new()
                        .write(true)
                        .create(true)
                        .mode(0o600)
                        .open("/etc/ppp/peers/hubeiexin").unwrap();

    let mut username = String::new();
    print!("> Username: ");
    io::stdout().flush().unwrap();
    io::stdin().read_line(&mut username).unwrap();


    let mut password = String::new();
    print!("> Password: ");
    io::stdout().flush().unwrap();
    io::stdin().read_line(&mut password).unwrap();


    let dir = fs::read_dir("/sys/class/net").unwrap();
    let names = dir.map(|one| {
        return one.unwrap()
                  .file_name().into_string().unwrap();
    }).filter(|one| {
        one.starts_with("e")
    }).collect::<Vec<String>>();

    for (i, name) in names.iter().enumerate() {
        println!("{} - {}", i, name);
    }

    let mut net_interface_choice;
    let mut choice:usize;
    loop {
        net_interface_choice = String::new();
        print!("> Please choose which network interface to use: ");
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut net_interface_choice).unwrap();
        choice = match net_interface_choice.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        if choice >= names.len() {
            continue;
        }
        break;
    }


    let ref net_interface = names[choice];
    let content = get_content(username.trim(),
                              password.trim(),
                              net_interface.trim());

    file.write_all(content.as_bytes()).unwrap();
}
