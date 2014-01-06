String.prototype.Trim = function () {
    return this.replace(/(^\s*)|(\s*$)/g, "");
}

function submit(address) {
    if (address.length > 0) {
        window.location.href = "rss_show.aspx?address=" + address;
    } else {
        alert("Please input the RSS address.");
    }
}

function reset() {
    document.getElementById("tx_rssaddress").value = "";
}

function update() {
    var flag = document.getElementById("flag").value.Trim();
    var oldkey = document.getElementById("hkey").value.Trim();
    var newkey = document.getElementById("mkey").value.Trim();

    var oldvalue = document.getElementById("hvalue").value.Trim();
    var newvalue = document.getElementById("mvalue").value.Trim();

    if (newkey.length == 0 || newvalue.length == 0) {
        alert("Please check your input, key and value are both need.");
        return;
    }

    if (flag == "update") {
        window.location.href = "process.aspx?operate=u"
            + "&oldkey=" + oldkey
            + "&newkey=" + newkey
            + "&oldvalue=" + oldvalue
            + "&newvalue=" + newvalue;
    } else if (flag == "create") {
        window.location.href = "process.aspx?operate=c"
            + "&newkey=" + newkey
            + "&newvalue=" + newvalue;
    }
}

function create() {
    document.getElementById("flag").value = "create";
    document.getElementById("d_modify").style.display = "inline";
}

function modify(key, value) {
    document.getElementById("flag").value = "update";

    document.getElementById("mkey").value = key;
    document.getElementById("hkey").value = key;
    document.getElementById("mvalue").value = value;
    document.getElementById("hvalue").value = value;

    document.getElementById("d_modify").style.display = "inline";
}

function cancel() {
    document.getElementById("d_modify").style.display = "none";

    document.getElementById("mkey").value = "";
    document.getElementById("hkey").value = "";
    document.getElementById("mvalue").value = "";
    document.getElementById("hvalue").value = "";
}