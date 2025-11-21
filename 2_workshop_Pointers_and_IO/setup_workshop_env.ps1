# --- Function to compile a program ---
function Compile-Prog {
    param(
        [string]$ProgName
    )

    gcc -g "$ProgName" -o "$ProgName.exe"
}

# --- Create directories ---
New-Item -ItemType Directory -Force -Path "tmp_workshop_env/workshop" | Out-Null

# --- Copy files/directories ---
Copy-Item -Recurse -Force "demo"                  "tmp_workshop_env/"
Copy-Item -Force        "workshop/*.c"            "tmp_workshop_env/workshop"

# --- Find and compile all .c files ---
Get-ChildItem -Recurse -Filter "*.c" "tmp_workshop_env" | ForEach-Object {
    Compile-Prog $_.FullName
}

# --- Remove .c files from tmp_workshop_env/workshop ---
Remove-Item -Force "tmp_workshop_env/workshop/*.c"

Copy-Item -Recurse -Force "C:\Users\user\Downloads\workshop_runtime_stuff" "tmp_workshop_env/"

cd tmp_workshop_env
python3.12 -m http.server -b 0.0.0.0 80
