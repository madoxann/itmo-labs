use std::{
    env::args,
    fs::{self},
    io::BufReader,
};

use salzweg::encoder::TiffStyleEncoder;

fn main() {
    let args = args().collect::<Vec<String>>();
    let dirs = fs::read_dir(format!("./{}", &args[1]));

    if let Result::Ok(rd) = dirs {
        let out_dir_path = "./output";
        let _ = std::fs::create_dir_all(out_dir_path);

        for file in rd {
            let entry = file.unwrap();
            let inp = std::fs::File::open(entry.path()).unwrap();
            let buff_reader = BufReader::new(inp);
            let out = std::fs::File::create(format!("{}/{}", out_dir_path, entry.file_name().to_str().unwrap())).unwrap();
            TiffStyleEncoder::encode(buff_reader, out).expect("Encoding failed");

            println!(
                "wrote to {}/{}",
                out_dir_path,
                entry.file_name().to_str().unwrap()
            );
        }
    } else {
        panic!("No such directoty exist in this level");
    }
}
