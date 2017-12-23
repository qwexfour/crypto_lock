package base;

public class FfEntity {
    private int id;
    private String surname;
    private String firstName;
    private String patronymic;
    private String publicKey;
    private String auth;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getPatronymic() {
        return patronymic;
    }

    public void setPatronymic(String patronymic) {
        this.patronymic = patronymic;
    }

    public String getPublicKey() {
        return publicKey;
    }

    public void setPublicKey(String publicKey) {
        this.publicKey = publicKey;
    }

    public String getAuth() {
        return auth;
    }

    public void setAuth(String auth) {
        this.auth = auth;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        FfEntity ffEntity = (FfEntity) o;

        if (id != ffEntity.id) return false;
        if (surname != null ? !surname.equals(ffEntity.surname) : ffEntity.surname != null) return false;
        if (firstName != null ? !firstName.equals(ffEntity.firstName) : ffEntity.firstName != null) return false;
        if (patronymic != null ? !patronymic.equals(ffEntity.patronymic) : ffEntity.patronymic != null) return false;
        if (publicKey != null ? !publicKey.equals(ffEntity.publicKey) : ffEntity.publicKey != null) return false;
        if (auth != null ? !auth.equals(ffEntity.auth) : ffEntity.auth != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = id;
        result = 31 * result + (surname != null ? surname.hashCode() : 0);
        result = 31 * result + (firstName != null ? firstName.hashCode() : 0);
        result = 31 * result + (patronymic != null ? patronymic.hashCode() : 0);
        result = 31 * result + (publicKey != null ? publicKey.hashCode() : 0);
        result = 31 * result + (auth != null ? auth.hashCode() : 0);
        return result;
    }
}
