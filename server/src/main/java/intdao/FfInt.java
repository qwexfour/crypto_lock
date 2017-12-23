package intdao;

import base.FfEntity;
import org.hibernate.Session;

public interface FfInt {
    public void save(FfEntity pricesEntity, Session session);
    public void delete(FfEntity pricesEntity, Session session);
    public void update(FfEntity pricesEntity, Session session);
    public FfEntity getEntityById(int id, Session session);
}
