let nasabahData = [];
let transaksiRiwayat = [];
let logAktivitas = [];
let adminLoggedIn = false;
let currentNasabah = null;

document.addEventListener('DOMContentLoaded', () => {
  // Load data dari localStorage (sama untuk admin & nasabah)
  let stored = localStorage.getItem('nasabahData');
  if (!stored || JSON.parse(stored).length === 0) {
    // Jika belum ada data di localStorage, coba load dari data_ktp.txt
    // Jika fetch gagal, fallback ke contoh kecil
    nasabahData = [];
    fetch('data_ktp.txt')
      .then(r => r.text())
      .then(text => {
        const parsed = parseDataKTP(text);
        if (parsed.length > 0) {
          nasabahData = parsed;
        } else {
          nasabahData = [
            { id: '1', nama: "Andi Prasetyo", saldo: 5000000 },
            { id: '2', nama: "Budi Santoso", saldo: 2500000 },
            { id: '3', nama: "Citra Dewi", saldo: 7500000 }
          ];
        }
        localStorage.setItem('nasabahData', JSON.stringify(nasabahData));
        // Jika sedang di admin dan halaman daftar aktif, refresh tabel
        if (window.location.pathname.includes('admin.html')) {
          if (adminLoggedIn && document.getElementById('lihat-daftar-nasabah') && document.getElementById('lihat-daftar-nasabah').classList.contains('active')) {
            loadNasabahTable();
          }
        }
      })
      .catch(() => {
        nasabahData = [
          { id: '1', nama: "Andi Prasetyo", saldo: 5000000 },
          { id: '2', nama: "Budi Santoso", saldo: 2500000 },
          { id: '3', nama: "Citra Dewi", saldo: 7500000 }
        ];
        localStorage.setItem('nasabahData', JSON.stringify(nasabahData));
        if (window.location.pathname.includes('admin.html')) {
          if (adminLoggedIn && document.getElementById('lihat-daftar-nasabah') && document.getElementById('lihat-daftar-nasabah').classList.contains('active')) {
            loadNasabahTable();
          }
        }
      });
  } else {
    nasabahData = JSON.parse(stored);
    // Jika data berasal dari localStorage dan masih menggunakan ID numerik pendek,
    // coba sinkronkan ke NIK yang ada di data_ktp.txt
    syncIdsWithKTP().catch(() => {});
  }

  transaksiRiwayat = JSON.parse(localStorage.getItem('transaksiRiwayat')) || [];
  logAktivitas = JSON.parse(localStorage.getItem('logAktivitas')) || [];
  adminLoggedIn = JSON.parse(localStorage.getItem('adminLoggedIn')) || false;

  // DETEKSI HALAMAN
  const path = window.location.pathname;

  if (path.includes('admin.html')) {
    // === ADMIN ===
    updateSidebar(); // untuk status & menu
    if (adminLoggedIn) {
      showSection('lihat-daftar-nasabah');
    } else {
      showSection('login-admin');
    }
  } else if (path.includes('nasabah.html')) {
    // === NASABAH ===
    updateSidebarNasabah();
    showSectionNasabah('login-nasabah'); // default buka login nasabah
  }
});

function simpanData() {
  localStorage.setItem('nasabahData', JSON.stringify(nasabahData));
  localStorage.setItem('transaksiRiwayat', JSON.stringify(transaksiRiwayat));
  localStorage.setItem('logAktivitas', JSON.stringify(logAktivitas));
}

// Parse isi data_ktp.txt menjadi array objek { id: NIK, nama, saldo }
function parseDataKTP(text) {
  const entries = text.split(/=+\s*/).filter(s => s.trim().length > 0);
  const result = [];
  entries.forEach(chunk => {
    // cari baris-baris penting
    const lines = chunk.split('\n').map(l => l.trim());
    let nik = '';
    let nama = '';
    let saldo = 0;
    lines.forEach(l => {
      if (l.startsWith('NIK')) {
        nik = l.split(':').slice(1).join(':').trim();
      }
      if (l.startsWith('Nama')) {
        nama = l.split(':').slice(1).join(':').trim();
      }
      if (l.startsWith('Saldo')) {
        const s = l.split(':').slice(1).join(':').trim().replace(/[^0-9\-]/g, '');
        saldo = parseInt(s) || 0;
      }
    });
    if (nik && nama) result.push({ id: String(nik), nama, saldo });
  });
  return result;
}

// Sinkronkan ID numerik pendek (1,2,3...) ke NIK bila cocok berdasarkan nama
async function syncIdsWithKTP() {
  try {
    const res = await fetch('data_ktp.txt');
    const text = await res.text();
    const ktpList = parseDataKTP(text);
    if (!ktpList || ktpList.length === 0) return;

    const needsMapping = nasabahData.some(n => /^[0-9]+$/.test(String(n.id)) && String(n.id).length <= 4);
    if (!needsMapping) return;

    let changed = false;
    nasabahData.forEach(n => {
      const name = (n.nama || '').trim().toLowerCase();
      const match = ktpList.find(k => (k.nama || '').trim().toLowerCase() === name);
      if (match && String(n.id) !== String(match.id)) {
        n.id = String(match.id);
        changed = true;
      }
    });

    if (changed) {
      simpanData();
      console.info('Nasabah IDs synced to NIK from data_ktp.txt');
      if (window.location.pathname.includes('admin.html')) {
        if (document.getElementById('lihat-daftar-nasabah') && document.getElementById('lihat-daftar-nasabah').classList.contains('active')) {
          loadNasabahTable();
        }
      }
    }
  } catch (e) {
    console.warn('Failed to sync IDs with KTP:', e);
  }
}

function tambahLog(aktivitas) {
  const waktu = new Date().toLocaleString('id-ID');
  logAktivitas.push({ waktu, aktivitas });
  simpanData();
}

function tambahRiwayat(nasabahId, jenis, jumlah, keterangan = "") {
  const tanggal = new Date().toLocaleString('id-ID');
  transaksiRiwayat.push({ nasabahId: String(nasabahId), tanggal, jenis, jumlah, keterangan });
  simpanData();
}

function updateSidebar() {
  const statusEl = document.getElementById('status-login');
  const sebelum = document.getElementById('menu-sebelum-login');
  const setelah = document.getElementById('menu-setelah-login');
  const logoutFooter = document.getElementById('footer-logout');

  if (adminLoggedIn) {
    statusEl.textContent = 'Status: Admin Aktif';
    sebelum.style.display = 'none';
    setelah.style.display = 'block';
    logoutFooter.style.display = 'block';
  } else {
    statusEl.textContent = 'Status: Belum Login';
    sebelum.style.display = 'block';
    setelah.style.display = 'none';
    logoutFooter.style.display = 'none';
  }
}

function showSection(id) {
  if (!adminLoggedIn && id !== 'login-admin') {
    alert('Silakan login admin terlebih dahulu!');
    showSection('login-admin');
    return;
  }

  document.querySelectorAll('#main-content section').forEach(sec => sec.classList.remove('active'));
  const target = document.getElementById(id);
  if (target) target.classList.add('active');

  // Highlight menu
  document.querySelectorAll('#menu-setelah-login a').forEach(a => a.classList.remove('active'));
  const link = document.querySelector(`#menu-setelah-login a[onclick="showSection('${id}')"]`);
  if (link) link.classList.add('active');

  // Refresh data
  if (id === 'lihat-daftar-nasabah') loadNasabahTable();
  if (id === 'laporan-total') updateLaporanTotal();
  if (id === 'statistik-saldo') updateStatistik();
  if (id === 'lihat-log') loadLogTable();
}

// Semua fungsi admin & nasabah dari kode bawahmu tetap sama
// (loginAdmin, logoutAdmin, tambahNasabah, hapusNasabah, cariNasabah, dll.)
// Copy-paste semua fungsi dari script.js yang kamu kirim di bagian bawah

// Pastikan ada CSS untuk .active di section

// === FUNGSI ADMIN ===
function loginAdmin() {
    const user = document.getElementById('admin-username').value;
    const pass = document.getElementById('admin-password').value;
    if (user === 'admin' && pass === '12345') {
        adminLoggedIn = true;
        localStorage.setItem('adminLoggedIn', 'true');
        tambahLog('Admin login berhasil');
        alert('Login Admin Berhasil!');
        updateSidebar();
        showSection('lihat-daftar-nasabah'); // ← Langsung ke daftar nasabah, bukan daftar baru
    } else {
        alert('Username atau password salah!');
    }
}

function logoutAdmin() {
    if (confirm('Yakin ingin logout?')) {
        adminLoggedIn = false;
        localStorage.removeItem('adminLoggedIn');
        tambahLog('Admin logout');
        updateSidebar();
        showSection('login-admin');
        document.getElementById('admin-username').value = '';
        document.getElementById('admin-password').value = '';
    }
}


async function loadNasabahTable(page = 1, perPage = 5) {
    if (!adminLoggedIn) return showSection('login-admin');
    const tbody = document.querySelector('#nasabah-table tbody');
    tbody.innerHTML = '';

    // Coba ambil semua data dari data_ktp.txt dan gunakan jika tersedia
    let dataToShow = nasabahData;
    try {
      const res = await fetch('data_ktp.txt');
      if (res.ok) {
        const text = await res.text();
        const parsed = parseDataKTP(text);
        if (parsed && parsed.length > 0) dataToShow = parsed;
      }
    } catch (e) {
      // fallback ke nasabahData jika fetch/parsing gagal
    }

    if (!dataToShow || dataToShow.length === 0) {
        const tr = document.createElement('tr');
        tr.innerHTML = '<td colspan="4" style="text-align:center; padding:60px; color:#888;">Belum ada nasabah terdaftar.</td>';
        tbody.appendChild(tr);
        document.getElementById('pagination') && (document.getElementById('pagination').innerHTML = '');
        return;
    }

    // Urutkan ascending berdasarkan ID/NIK (numeric jika memungkinkan)
    dataToShow.sort((a, b) => {
        const na = Number(a.id);
        const nb = Number(b.id);
        if (!isNaN(na) && !isNaN(nb)) return na - nb;
        return String(a.id).localeCompare(String(b.id), undefined, {numeric: true});
    });

    // Pagination
    const total = dataToShow.length;
    const totalPages = Math.max(1, Math.ceil(total / perPage));
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;
    const start = (page - 1) * perPage;
    const slice = dataToShow.slice(start, start + perPage);

    slice.forEach(n => {
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td><strong>${n.id}</strong></td>
            <td>${n.nama}</td>
            <td>Rp ${n.saldo.toLocaleString('id-ID')}</td>
        `;
        tbody.appendChild(tr);
    });

    renderPagination(total, page, perPage);
}

function renderPagination(totalItems, currentPage, perPage) {
  const container = document.getElementById('pagination');
  if (!container) return;
  const totalPages = Math.max(1, Math.ceil(totalItems / perPage));
  container.innerHTML = '';

  const prev = document.createElement('button');
  prev.textContent = '‹ Prev';
  prev.style.padding = '8px 12px';
  prev.style.borderRadius = '6px';
  prev.style.border = '1px solid #ddd';
  prev.disabled = currentPage === 1;
  prev.onclick = () => loadNasabahTable(Math.max(1, currentPage - 1), perPage);
  container.appendChild(prev);

  const info = document.createElement('span');
  info.textContent = `Halaman ${currentPage} dari ${totalPages}`;
  info.style.alignSelf = 'center';
  info.style.margin = '0 12px';
  container.appendChild(info);

  const next = document.createElement('button');
  next.textContent = 'Next ›';
  next.style.padding = '8px 12px';
  next.style.borderRadius = '6px';
  next.style.border = '1px solid #ddd';
  next.disabled = currentPage === totalPages;
  next.onclick = () => loadNasabahTable(Math.min(totalPages, currentPage + 1), perPage);
  container.appendChild(next);
}

function tambahNasabah() {
    if (!adminLoggedIn) {
        alert('Login admin dulu!');
        showSection('login-admin');
        return;
    }

    const nama = document.getElementById('new-nama').value.trim();
    const saldo = parseInt(document.getElementById('new-saldo').value) || 0;

    if (!nama) {
        alert('Nama wajib diisi!');
        return;
    }

    // Buat ID baru (jika ada ID numerik gunakan increment, jika tidak gunakan timestamp)
    const numericIds = nasabahData.map(n => Number(n.id)).filter(x => !isNaN(x));
    const newId = numericIds.length > 0 ? (Math.max(...numericIds) + 1).toString() : Date.now().toString();

    // Tambah ke array (simpan ID sebagai string untuk konsistensi dengan NIK)
    nasabahData.push({ id: String(newId), nama, saldo });

    // Simpan ke localStorage
    simpanData();

    // Tambah log
    tambahLog(`Admin menambahkan nasabah baru: ${nama} (ID: ${newId})`);

    // Beri feedback
    alert(`Nasabah ${nama} berhasil didaftarkan dengan ID ${newId}!`);

    // Kosongkan form
    document.getElementById('new-nama').value = '';
    document.getElementById('new-saldo').value = '0';

    // Otomatis refresh tabel kalau sedang di halaman daftar nasabah
    if (document.getElementById('lihat-daftar-nasabah').classList.contains('active')) {
        loadNasabahTable();
    }
}

function hapusNasabah(id) {
  if (!adminLoggedIn) return alert('Login admin dulu!');
  if (!id) id = document.getElementById('hapus-id') ? document.getElementById('hapus-id').value.trim() : '';
  if (!id) return alert('Masukkan ID/NIK nasabah yang ingin dihapus!');
  if (!confirm('Yakin hapus nasabah ini?')) return;
  const nasabah = nasabahData.find(n => String(n.id) === String(id));
  if (!nasabah) return alert('ID/NIK tidak ditemukan!');
  nasabahData = nasabahData.filter(n => String(n.id) !== String(id));
  tambahLog(`Admin menghapus nasabah: ${nasabah.nama}`);
  simpanData();
  loadNasabahTable();
}

function cariNasabah() {
    if (!adminLoggedIn) return alert('Login admin dulu!');
    const nama = document.getElementById('cari-nama').value.toLowerCase();
    const hasil = nasabahData.filter(n => n.nama.toLowerCase().includes(nama));
    
    const div = document.getElementById('hasil-cari');
    if (hasil.length === 0) {
        div.innerHTML = '<p>Tidak ditemukan.</p>';
    } else {
        let html = '<ul>';
        hasil.forEach(n => {
            html += `<li>ID: ${n.id} | ${n.nama} | Saldo: Rp ${n.saldo.toLocaleString('id-ID')}</li>`;
        });
        html += '</ul>';
        div.innerHTML = html;
    }
}

function updateLaporanTotal() {
    // Prefer data from data_ktp.txt when available
    (async () => {
      let data = nasabahData;
      try {
        const res = await fetch('data_ktp.txt');
        if (res.ok) {
          const text = await res.text();
          const parsed = parseDataKTP(text);
          if (parsed && parsed.length > 0) data = parsed;
        }
      } catch (e) {
        // ignore and use nasabahData
      }

      const count = data.length;
      const total = data.reduce((sum, n) => sum + (Number(n.saldo) || 0), 0);
      document.getElementById('total-nasabah').textContent = count;
      document.getElementById('total-saldo').textContent = 'Rp ' + total.toLocaleString('id-ID');
    })();
}

function updateStatistik() {
    (async () => {
      let data = nasabahData;
      try {
        const res = await fetch('data_ktp.txt');
        if (res.ok) {
          const text = await res.text();
          const parsed = parseDataKTP(text);
          if (parsed && parsed.length > 0) data = parsed;
        }
      } catch (e) {
        // fallback to nasabahData
      }

      if (!data || data.length === 0) {
        document.getElementById('saldo-max').textContent = '0';
        document.getElementById('saldo-min').textContent = '0';
        document.getElementById('saldo-rata').textContent = '0';
        return;
      }

      const saldos = data.map(n => Number(n.saldo) || 0);
      const max = Math.max(...saldos);
      const min = Math.min(...saldos);
      const rata = Math.round(saldos.reduce((a,b) => a+b, 0) / saldos.length);

      document.getElementById('saldo-max').textContent = 'Rp ' + max.toLocaleString('id-ID');
      document.getElementById('saldo-min').textContent = 'Rp ' + min.toLocaleString('id-ID');
      document.getElementById('saldo-rata').textContent = 'Rp ' + rata.toLocaleString('id-ID');
    })();
}

function backupData() {
    const data = {
        nasabah: nasabahData,
        transaksi: transaksiRiwayat,
        backupTime: new Date().toISOString()
    };
    const blob = new Blob([JSON.stringify(data, null, 2)], {type: 'application/json'});
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'backup_bank_' + new Date().toISOString().slice(0,10) + '.json';
    a.click();
    document.getElementById('backup-status').textContent = 'Backup berhasil di-download!';
    tambahLog('Admin melakukan backup data');
}

function editNasabah() {
  if (!adminLoggedIn) return alert('Login admin dulu!');
  const id = document.getElementById('edit-id').value.trim();
  const nama = document.getElementById('edit-nama').value.trim();
  const saldo = parseInt(document.getElementById('edit-saldo').value);

  const nasabah = nasabahData.find(n => String(n.id) === String(id));
  if (!nasabah) return alert('ID/NIK tidak ditemukan!');

  if (nama) nasabah.nama = nama;
  if (!isNaN(saldo)) nasabah.saldo = saldo;

  tambahLog(`Admin mengedit nasabah ID ${id}`);
  simpanData();
  alert('Data berhasil diupdate!');
}

function loadLogTable() {
    const tbody = document.querySelector('#log-table tbody');
    tbody.innerHTML = '';
    logAktivitas.slice().reverse().forEach(log => {
        const tr = document.createElement('tr');
        tr.innerHTML = `<td>${log.waktu}</td><td>${log.aktivitas}</td>`;
        tbody.appendChild(tr);
    });
}

// === FUNGSI NASABAH ===
async function loginNasabah() {
  const id = document.getElementById('nasabah-id-login').value.trim();

  // Coba load data_ktp.txt terlebih dahulu dan gunakan sebagai sumber kebenaran
  try {
    const res = await fetch('data_ktp.txt');
    if (res.ok) {
      const text = await res.text();
      const parsed = parseDataKTP(text);
      if (parsed && parsed.length > 0) {
        nasabahData = parsed;
        // simpan ke localStorage supaya semua operasi menggunakan data yang sama
        simpanData();
      }
    }
  } catch (e) {
    // jika fetch gagal, lanjut menggunakan nasabahData yang ada
  }

  currentNasabah = nasabahData.find(n => String(n.id) === String(id));
  if (currentNasabah) {
    tambahLog(`Nasabah ${currentNasabah.nama} (ID: ${id}) login`);
    alert(`Selamat datang, ${currentNasabah.nama}!`);
    updateSidebarNasabah();
    showSectionNasabah('cek-saldo');
  } else {
    alert('NIK Nasabah tidak ditemukan!');
  }
}

function logoutNasabah() {
  if (confirm('Yakin ingin logout?')) {
    currentNasabah = null;
    tambahLog('Nasabah logout');
    updateSidebarNasabah();
    showSectionNasabah('login-nasabah');
    document.getElementById('nasabah-id-login').value = '';
  }
}

function updateCekSaldo() {
    if (!currentNasabah) return showSection('login-nasabah');
    document.getElementById('nama-nasabah').textContent = currentNasabah.nama;
    document.getElementById('saldo-nasabah').textContent = 'Rp ' + currentNasabah.saldo.toLocaleString('id-ID');
}

function setorUang() {
    if (!currentNasabah) return alert('Login dulu!');
    const jumlah = parseInt(document.getElementById('jumlah-setor').value);
    if (isNaN(jumlah) || jumlah <= 0) return alert('Masukkan jumlah valid!');
    
    currentNasabah.saldo += jumlah;
    tambahRiwayat(currentNasabah.id, 'Setor', jumlah);
    tambahLog(`Nasabah ${currentNasabah.nama} setor Rp ${jumlah.toLocaleString()}`);
    simpanData();
    alert('Setor berhasil!');
    updateCekSaldo();
    document.getElementById('jumlah-setor').value = '';
}

function tarikUang() {
    if (!currentNasabah) return alert('Login dulu!');
    const jumlah = parseInt(document.getElementById('jumlah-tarik').value);
    if (isNaN(jumlah) || jumlah <= 0) return alert('Masukkan jumlah valid!');
    if (jumlah > currentNasabah.saldo) return alert('Saldo tidak cukup!');
    
    currentNasabah.saldo -= jumlah;
    tambahRiwayat(currentNasabah.id, 'Tarik', jumlah);
    tambahLog(`Nasabah ${currentNasabah.nama} tarik Rp ${jumlah.toLocaleString()}`);
    simpanData();
    alert('Penarikan berhasil!');
    updateCekSaldo();
    document.getElementById('jumlah-tarik').value = '';
}

function transferUang() {
  if (!currentNasabah) return alert('Login dulu!');
  const tujuanId = document.getElementById('tujuan-id').value.trim();
  const jumlah = parseInt(document.getElementById('jumlah-transfer').value);

  if (isNaN(jumlah) || jumlah <= 0) return alert('Jumlah tidak valid!');
  if (jumlah > currentNasabah.saldo) return alert('Saldo tidak cukup!');
  if (String(tujuanId) === String(currentNasabah.id)) return alert('Tidak bisa transfer ke diri sendiri!');

  const tujuan = nasabahData.find(n => String(n.id) === String(tujuanId));
  if (!tujuan) return alert('ID/NIK tujuan tidak ditemukan!');

  currentNasabah.saldo -= jumlah;
  tujuan.saldo += jumlah;

  const ket = `Transfer ke ${tujuan.nama}`;
  tambahRiwayat(currentNasabah.id, 'Transfer Keluar', jumlah, ket);
  tambahRiwayat(tujuan.id, 'Transfer Masuk', jumlah, `Dari ${currentNasabah.nama}`);
  tambahLog(`Transfer: ${currentNasabah.nama} → ${tujuan.nama} Rp ${jumlah.toLocaleString()}`);

  simpanData();
  alert('Transfer berhasil!');
  updateCekSaldo();
}

function cetakStruk() {
    if (!currentNasabah) return;
    const riwayatTerakhir = transaksiRiwayat
      .filter(t => String(t.nasabahId) === String(currentNasabah.id))
      .slice(-1)[0];
    
    let struk = "=== STRUK TRANSAKSI ===\n";
    struk += `Nasabah: ${currentNasabah.nama}\n`;
    struk += `ID      : ${currentNasabah.id}\n`;
    struk += `Saldo   : Rp ${currentNasabah.saldo.toLocaleString('id-ID')}\n\n`;
    
    if (riwayatTerakhir) {
        struk += `Transaksi Terakhir:\n`;
        struk += `${riwayatTerakhir.tanggal}\n`;
        struk += `${riwayatTerakhir.jenis} Rp ${riwayatTerakhir.jumlah.toLocaleString('id-ID')}\n`;
        if (riwayatTerakhir.keterangan) struk += `${riwayatTerakhir.keterangan}\n`;
    } else {
        struk += "Belum ada transaksi.\n";
    }
    
    struk += "\nTerima kasih!\nNeo Bank";
    
    document.getElementById('struk-terakhir').textContent = struk;
    alert('Struk siap dicetak! (Copy teks di atas)');
}

function updateStatusAkun() {
    if (!currentNasabah) return showSection('login-nasabah');
    document.getElementById('status-id').textContent = currentNasabah.id;
    document.getElementById('status-nama').textContent = currentNasabah.nama;
  const saldoEl = document.getElementById('status-saldo');
  if (saldoEl) saldoEl.textContent = 'Rp ' + (Number(currentNasabah.saldo) || 0).toLocaleString('id-ID');
}

function loadRiwayatNasabah() {
    if (!currentNasabah) return showSection('login-nasabah');
    const tbody = document.querySelector('#riwayat-table tbody');
    tbody.innerHTML = '';
    
    const riwayat = transaksiRiwayat
      .filter(t => String(t.nasabahId) === String(currentNasabah.id))
      .slice().reverse();
    
    if (riwayat.length === 0) {
        tbody.innerHTML = '<tr><td colspan="4">Belum ada transaksi.</td></tr>';
        return;
    }
    
    riwayat.forEach(t => {
        const tr = document.createElement('tr');
        tr.innerHTML = `<td>${t.tanggal}</td><td>${t.jenis}</td>
                        <td>Rp ${t.jumlah.toLocaleString('id-ID')}</td>
                        <td>${t.keterangan || '-'}</td>`;
        tbody.appendChild(tr);
    });
}

function showSectionNasabah(id) {
  if (!currentNasabah && id !== 'login-nasabah') {
    alert('Silakan login sebagai nasabah terlebih dahulu!');
    showSectionNasabah('login-nasabah');
    return;
  }

  document.querySelectorAll('#main-content section').forEach(sec => {
    sec.classList.remove('active');
  });
  const target = document.getElementById(id);
  if (target) target.classList.add('active');

  // Refresh data
  if (id === 'cek-saldo') updateCekSaldo();
  if (id === 'lihat-riwayat') loadRiwayatNasabah();
  if (id === 'cek-status-akun') updateStatusAkun();
}

function updateSidebarNasabah() {
  const statusEl = document.getElementById('status-nasabah');
  if (currentNasabah) {
    statusEl.textContent = `Selamat datang, ${currentNasabah.nama}`;
    document.getElementById('menu-sebelum-login').style.display = 'none';
    document.getElementById('menu-setelah-login').style.display = 'block';
    document.getElementById('footer-logout').style.display = 'block';
  } else {
    statusEl.textContent = 'Status: Belum Login';
    document.getElementById('menu-sebelum-login').style.display = 'block';
    document.getElementById('menu-setelah-login').style.display = 'none';
    document.getElementById('footer-logout').style.display = 'none';
  }
}

// // Inisialisasi
// showSection('login-admin');